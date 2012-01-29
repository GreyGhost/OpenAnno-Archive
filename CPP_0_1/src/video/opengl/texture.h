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

#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef _WIN32
# include <SDL.h>
#else
# include <SDL/SDL.h>
#endif
#include <GL/gl.h>
#include <string>

#include "io/resource.h"

class Texture
{
public:
    Texture();
    virtual ~Texture();
    bool use() const;
    int getWidth() const { return m_width; };
    int getHeight() const { return m_height; };
    float getTexCoordX() const { return m_texCoordX; };
    float getTexCoordY() const { return m_texCoordY; };
	bool createFromSdlSurface(SDL_Surface* surface);
	void free();
protected:
    int m_width, m_height;
    int m_realWidth, m_realHeight;
    float m_texCoordX, m_texCoordY;
    GLuint m_glTex;
};

#endif
