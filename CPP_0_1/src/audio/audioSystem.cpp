/***************************************************************************
 *   Copyright (C) 2006 by Alexander Couzens							   *
 *   acouzens@gmx.net                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "audio/audioSystem.h"

#include "io/resourceManager.h"
#include "io/resourceLoader.h"

#ifdef _WIN32
# include <SDL.h>
# include <SDL_mixer.h>
#else
# include <SDL/SDL.h>
# include <SDL/SDL_mixer.h>
#endif

AudioSystem* AudioSystem::m_instance = NULL;

AudioSystem::AudioSystem()
{
	m_isActive = true;
	isInit = false;
}

AudioSystem::~AudioSystem()
{
}

void AudioSystem::initInstance(AudioSystem* instance)
{
	m_instance = instance;
}

AudioSystem* AudioSystem::getInstance()
{
	return m_instance;
}

void AudioSystem::freeInstance()
{
	delete m_instance;
	m_instance = NULL;
}

bool AudioSystem::init(int frequency, bool use16bit, unsigned int numOutputChannels, unsigned int bufferSize, unsigned int maxNumSampleChannels)
{
	if (isInit) {
		Mix_CloseAudio();
		isInit = false;
	} else {
		if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
			std::cout << "SDL audio initialization failed: " << SDL_GetError() << std::endl;
			return false;
		}

		ResourceManager::getInstance()->addResourceLoader(new GenericResourceLoader<Sample, Resource::SAMPLE>());
		ResourceManager::getInstance()->addResourceLoader(new GenericResourceLoader<Music, Resource::MUSIC>());
	}

	Uint16 format = use16bit ? AUDIO_S16SYS : AUDIO_S8;
	if (Mix_OpenAudio(frequency, format, numOutputChannels, bufferSize) < 0) {
		Mix_CloseAudio();
		std::cout << "failed to init sdl audio system: " << SDL_GetError() << std::endl;
		return false;
	}

	Mix_AllocateChannels(maxNumSampleChannels);

	int audio_frq, audio_channels;
	unsigned short audio_format;
	// print out some info on the audio device and stream
	Mix_QuerySpec(&audio_frq, &audio_format, &audio_channels);
    int bits = audio_format&0xFF;

	std::string audio_channels_name;
	if (audio_channels > 3) {
		audio_channels_name = "surround";
	} else if (audio_channels > 1) {
		audio_channels_name = "stereo";
	} else {
		audio_channels_name = "mono";
	}

	const int audioDriverNameBufferSize=256;
	char audioDriverNameBuffer[audioDriverNameBufferSize];

	std::cout << "Audio System" << std::endl;
	std::cout << "  Drivername: "	<< SDL_AudioDriverName(audioDriverNameBuffer, audioDriverNameBufferSize) << std::endl;
	std::cout << "  Frequency: "	<< audio_frq << " Hz" << std::endl;
	std::cout << "  Sample Rate: "	<< bits << " bits" << std::endl;
	std::cout << "  Output channels: " << audio_channels << " (" << audio_channels_name << ")" << std::endl;
	std::cout << "  Buffersize: "	<< bufferSize << " bytes (latency: " << ((float)bufferSize/audio_frq*1000) << "ms)" << std::endl;

	isInit = true;

	return true;
}


void AudioSystem::setGlobalSoundVolume(float volume)
{
	globalSoundVolume = volume;
}

void AudioSystem::setGlobalMusicVolume(float volume)
{
	globalMusicVolume = volume;
}

void AudioSystem::activate()
{
	m_isActive = true;
	Mix_ResumeMusic();
}

void AudioSystem::deactivate()
{
	m_isActive = false;
	Mix_HaltChannel(-1);
	Mix_PauseMusic();
}

bool AudioSystem::playingMusic() const
{
	return Mix_PlayingMusic();
}

bool AudioSystem::playMusic(MusicPtr music, int numLoops, float volume) const
{
	currentMusicVolume = volume;
	if (m_isActive)
		return music->play(numLoops, volume * globalMusicVolume);
	return true;
}

MusicPtr AudioSystem::createMusic(const std::string& name, const std::string& filename)
{
	if (!isInit)
		return MusicPtr(0);
	MusicPtr ret = new Music(filename);
	if (!ret->load())
	{
		return MusicPtr(0);
	}
	return ret;
}
