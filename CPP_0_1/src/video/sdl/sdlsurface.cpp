/***************************************************************************
 *   Copyright (C) 2006 by Lars-Peter Clausen							   *
 *   lars@laprican.de                                                      *
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

#include <iostream>

#include "video/sdl/sdlsurface.h"
#include "video/sdl/sdlvideosystem.h"

#include "io/resourceManager.h"

SDLSurface::SDLSurface(const std::string& filename) : Surface(filename)
{
}

SDLSurface::~SDLSurface()
{
    if (m_isLoaded) {
        unload();
    }
}

bool SDLSurface::load()
{
    if (!m_isLoaded)
    {
		SDL_RWops* fileOps = getRWops();
		if(!fileOps)
		{
			std::cout << "Couldn't open texture \"" << getName() << "\": " << IMG_GetError() << std::endl;
			return false;
		}
		SDL_Surface* temp = IMG_Load_RW(fileOps, 1);

        if (temp == NULL) {
            std::cout << "Couldn't open texture \"" << getName() << "\": " << IMG_GetError() << std::endl;
            return false;
        }

		m_surface = SDL_DisplayFormatAlpha(temp);
		if(m_surface == NULL)
		{
			m_surface = temp;
		}
		else
		{
			SDL_FreeSurface(temp);
		}
		SDL_SetColorKey(m_surface, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB(m_surface->format, 255, 0, 255));
		m_isLoaded = true;
    }
    return m_isLoaded;
}

void SDLSurface::unload()
{
	if (m_isLoaded)
	{
		m_isLoaded = false;
		SDL_FreeSurface(m_surface);
	}
}
