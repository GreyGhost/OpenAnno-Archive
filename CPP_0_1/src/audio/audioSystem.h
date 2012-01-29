/***************************************************************************
 *   Copyright (C) 2006 by Stefan Geiger                                   *
 *   stegei@real-world.ch                                                  *
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

#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include <iostream>

#include "audio/music.h"
#include "audio/sample.h"

class Sample;
class Music;

class AudioSystem
{
public:
	AudioSystem();
	~AudioSystem();

	static void initInstance(AudioSystem* instance);
	static AudioSystem* getInstance();
	static void freeInstance();

	bool init(int frequency, bool use16bit, unsigned int numOutputChannels, unsigned int bufferSize, unsigned int maxNumSampleChannels);
	SamplePtr createSample(const std::string& name, const std::string& filename);
	MusicPtr createMusic(const std::string& name, const std::string& filename);

	/**
	 * Plays a sample
	 *
	 * @param pSampe sample to play
	 * @param numLoops number of loops (eg. 3 plays sample four times), -1 plays infinitely
	 * @param volume volume
	 * @return returns true if successful otherwise false.
	 */
	bool playSample(SamplePtr pSample, int numLoops=0, float volume=1.0f) const ;
	bool playMusic(MusicPtr pMusic, int numLoops=-1, float volume=1.0f) const;
	
	bool playingMusic() const;

	/**
	* sets the global sound volume
	*
	* @param volume volume to set 0-1
	*/
	void setGlobalSoundVolume(float volume);
	void setGlobalMusicVolume(float volume);

	void activate();
	void deactivate();
	
private:
	static AudioSystem* m_instance;
	bool isInit;
	mutable float currentMusicVolume;
	float globalSoundVolume;
	float globalMusicVolume;
	bool m_isActive;
};

#endif // AUDIOSYSTEM_H
