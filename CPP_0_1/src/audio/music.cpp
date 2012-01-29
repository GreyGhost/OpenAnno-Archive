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

#include "audio/music.h"

Music::Music(const std::string& name) : Resource(name, MUSIC, true)
{
	m_Music = NULL;
}

Music::~Music()
{
	unload();
}

bool Music::load()
{
	m_isLoaded = true;
	if (m_Music == NULL) {
		SDL_RWops* fileOps = getRWops();
		if(!fileOps)
		{
			return false;
		}
		m_Music = Mix_LoadMUS_RW(fileOps);

		if (m_Music == NULL) {
			return false;
		}
		Mix_VolumeMusic(SDL_MIX_MAXVOLUME);
	}
	return true;
}

void Music::unload()
{
	if (m_Music)
		Mix_FreeMusic(m_Music);
}

bool Music::play(int numLoops, float volume)
{
	if (m_Music) {
		if (Mix_PlayMusic(m_Music, numLoops) == -1) {
			return false;
		}
		Mix_VolumeMusic((int)(volume*SDL_MIX_MAXVOLUME) );
		return true;
	}
	return false;
}

bool Music::resume()
{
	if (m_Music) {
		Mix_ResumeMusic();
		return true;
	}
	return false;
}

bool Music::pause()
{
	if (m_Music) {
		Mix_PauseMusic();
		return true;
	}
	return false;
}

bool Music::stop()
{
	if (m_Music) {
		Mix_HaltMusic();
		return true;
	}
	return false;
}
