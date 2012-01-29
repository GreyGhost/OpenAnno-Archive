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
#ifdef _WIN32
# include <SDL_image.h>
#else
# include <SDL/SDL_image.h>
#endif

#include "video/opengl/openglsurface.h"
#include "video/opengl/openglvideosystem.h"
#include "video/sdl/sdlvideosystem.h"

#include "io/resourceManager.h"

OpenGLSurface::OpenGLSurface(const std::string& filename) : Surface(filename)
{
}

OpenGLSurface::~OpenGLSurface()
{
    if (m_isLoaded)
    {
        unload();
    }
}

bool OpenGLSurface::load()
{
    if (!m_isLoaded)
    {
		if(!reload())
		{
			return false;
		}
    }
    return m_isLoaded;
}

bool OpenGLSurface::reload()
{
	m_isLoaded = false;
	SDL_RWops* fileOps = getRWops();
	if(!fileOps)
	{
		std::cout << "Couldn't open texture \"" << getName() << "\": " << IMG_GetError() << std::endl;
		return false;
	}
	SDL_Surface* surface = IMG_Load_RW(fileOps, 1);

	if (surface == NULL) {
		std::cout << "Couldn't open texture \"" << getName() << "\": " << IMG_GetError() << std::endl;
		return false;
	}
	m_isLoaded = texture.createFromSdlSurface(surface);

	return true;
}

void OpenGLSurface::unload()
{
    if (m_isLoaded) {
		m_isLoaded = false;
		texture.free();
    }
}
