/***************************************************************************
 *   Copyright (C) 2006 by Jonathan Sieber                                 *
 *   jonathan_sieber@yahoo.de                                              *
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

#ifdef _WIN32
# include <SDL.h>
# include <SDL_mixer.h>
#else
# include <SDL/SDL.h>
# include <SDL/SDL_mixer.h>
#endif

#include "audio/sample.h"
#include "io/resourceManager.h"

Sample::Sample(const char* pName) : Resource(pName, Resource::SAMPLE, true)
{
	m_Chunk = 0;
	m_Channel = -1;
}

Sample::~Sample()
{
	unload();
}

bool Sample::load()
{
	m_isLoaded = true;
	if (m_Chunk == 0)
	{
		m_Chunk = Mix_LoadWAV(getName().c_str());
		if (m_Chunk == 0)
			return false;
	}
	
	Mix_VolumeChunk(m_Chunk, SDL_MIX_MAXVOLUME);
	return true;
}

void Sample::unload()
{
	if (m_Chunk)
	{
		Mix_FreeChunk(m_Chunk);
		m_Chunk = 0;
	}
}

bool Sample::play(int numLoops, float volume)
{
	if (m_Chunk != NULL) {
		m_Channel = Mix_PlayChannel(-1, m_Chunk, numLoops);

		if (m_Channel == -1) {
			return false;
		}
		Mix_Volume(m_Channel, (int)(volume*SDL_MIX_MAXVOLUME) );
		return true;
	}
	return false;
}

bool Sample::stop()
{
	if (m_Channel != -1) {
		Mix_HaltChannel(m_Channel);
		m_Channel = -1;
        return true;
    }
    return false;
}

void Sample::setVolume(float volume)
{
	if (m_Channel != -1) {
		Mix_Volume(m_Channel, (int)(volume*SDL_MIX_MAXVOLUME) );
	}
}

void Sample::setPanning(float left, float right)
{
	if (m_Channel != -1) {
		Mix_SetPanning(m_Channel, (Uint8)(left*255), (Uint8)(right*255) );
	}
}
