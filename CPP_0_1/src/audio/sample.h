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

#ifndef SAMPLE_H
#define SAMPLE_H

#include "io/resource.h"
#include "io/refptr.h"

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef _WIN32
# include <SDL_mixer.h>
#else
# include <SDL/SDL_mixer.h>
#endif

class Sample : public Resource
{
public:
	Sample(const char* pName);
	virtual ~Sample();

	
	bool play(int numLoops, float volume);
			
	/**
	 * Stop playing
	 */
	bool stop();

	/**
	 * Set volume, only has an effect when called while sample is being played
	 *
	 * @param volume volume level to set (0 quiet, 1 max)
	 */
	void setVolume(float volume);
	/**
	 * Set the panning, only has an effect when called while sample is being played
	 *
	 * @param left channel volume
	 * @param right channel volume
	 */
	void setPanning(float left, float right);

	virtual const char* getClassName() const { return "Sample"; }

	virtual bool load();
	virtual void unload();

private:
	Mix_Chunk* m_Chunk;
	int m_Channel;
};

typedef RefPtr<Sample> SamplePtr;

#endif // SAMPLE_H
