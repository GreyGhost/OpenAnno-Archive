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

#ifndef MUSIC_H
#define MUSIC_H

#include "io/resource.h"
#include "io/refptr.h"

#define USE_RWOPS

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef _WIN32
# include <SDL_mixer.h>
#else
# include <SDL/SDL_mixer.h>
#endif


class Music : public Resource
{
public:
	Music(const std::string& name);
	~Music();

	bool resume();
	bool pause();
	bool stop();
	bool play(int numLoops, float volume);

	const char* getClassName() const { return "Music"; }

	virtual bool load();
	virtual void unload();

private:
	Mix_Music* m_Music;
};

typedef RefPtr<Music> MusicPtr;

#endif // MUSIC_H
