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

#include "texture.h"
#include <assert.h>
#include <GL/gl.h>
#ifdef _WIN32
# include <SDL.h>
# include <SDL_image.h>
#else
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
#endif
#include <string>
#include <iostream>

Texture::Texture() : m_width(0), m_height(0), m_realWidth(0), m_realHeight(0), m_texCoordX(0.0f), m_texCoordY(0.0f), m_glTex(0)
{
}

Texture::~Texture()
{
	if(m_glTex)
	{
         glDeleteTextures(1, &m_glTex);
	}
}

bool Texture::createFromSdlSurface(SDL_Surface* surface)
{
	if(surface->w == 0 || surface->h == 0)
		return false;

    m_realWidth = 1;
    m_realHeight = 1;

    m_width = surface->w;
    m_height = surface->h;

    while(m_realWidth < m_width) m_realWidth = m_realWidth << 1;
    while(m_realHeight < m_height) m_realHeight = m_realHeight << 1;

    m_texCoordX = m_width / static_cast<float>(m_realWidth);
    m_texCoordY = m_height / static_cast<float>(m_realHeight);

    if((surface->format->BytesPerPixel != 4 && surface->format->BytesPerPixel != 3) || m_realWidth != m_width || m_realHeight != m_height)
    {
        #if SDL_BYTEORDER == SDL_LIL_ENDIAN
			SDL_Surface* temp = SDL_CreateRGBSurface(SDL_SWSURFACE, m_realWidth, m_realHeight, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
        #else
			SDL_Surface* temp = SDL_CreateRGBSurface(SDL_SWSURFACE, m_realWidth, m_realHeight, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
        #endif

        SDL_SetAlpha(surface, 0, 0);
        SDL_BlitSurface(surface, 0, temp, 0);
        SDL_FreeSurface(surface);
        surface = temp;
    }

    glGenTextures(1, &m_glTex);

    if(!m_glTex)
    {
        std::cout << "Couldn't create OpenGL texture: " << glGetError() << std::endl;
        return false;
    }

    GLenum format = 0;
    if(surface->format->BytesPerPixel == 3)
        format = GL_RGB;
    else if(surface->format->BytesPerPixel == 4)
        format = GL_RGBA;
    else
        assert(0);

    glBindTexture(GL_TEXTURE_2D, m_glTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_realWidth, m_realHeight, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
    SDL_FreeSurface(surface);

    return true;
}

bool Texture::use() const
{
    if(m_glTex)
    {
        glBindTexture(GL_TEXTURE_2D, m_glTex);
		return true;
	}
	return false;
}

void Texture::free()
{
	if(m_glTex)
	{
         glDeleteTextures(1, &m_glTex);
         m_glTex = 0;
	}
}
