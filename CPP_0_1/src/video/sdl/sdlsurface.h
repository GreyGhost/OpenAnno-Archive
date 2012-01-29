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
#ifndef SDLSURFACE_H
#define SDLSURFACE_H

#include "io/refptr.h"
#include "video/surface.h"
#ifdef _WIN32
# include <SDL_image.h>
#else
# include <SDL/SDL_image.h>
#endif

class SDLSurface : public Surface
{
friend class SDLVideoSystem;
public:
	SDLSurface(const std::string& filename);
	~SDLSurface();

    int getWidth() const { if(m_surface) return m_surface->w; return 0; };
    int getHeight() const { if(m_surface) return m_surface->h; return 0; };
    bool load();
    void unload();

private:
    SDL_Surface* m_surface;
};

typedef RefPtr<SDLSurface> SDLSurfacePtr;

#endif
