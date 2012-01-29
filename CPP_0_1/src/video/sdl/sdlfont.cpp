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
#include <string>
//#include <SDL/SDL_ttf.h>
#include "video/sdl/sdlfont.h"
#include "video/sdl/sdlvideosystem.h"

SDLFont::SDLFont(const std::string& filename) : Font(filename)
{
}


SDLFont::~SDLFont()
{
	if (m_isLoaded)
	{
		unload();
	}
}

void SDLFont::drawText(const VideoSystem* const screen, std::string text, int x, int y, const Color& color, Size size, const Rect* const rect) const
{
	if(!ttfFont[size])
	{
		return;
	}

	SDL_Color color2 = {0,0,0};
	color2.r = color.cRed;
	color2.g = color.cGreen;
	color2.b = color.cBlue;

	SDL_Rect destRect, srcRect;
	destRect.x = (Sint16) x;
	destRect.y = (Sint16) y;
	SDL_Surface* sur = TTF_RenderUTF8_Blended(ttfFont[size], text.c_str(), color2);
	if(rect)
	{
		srcRect.x = (Sint16) rect->x;
		srcRect.y = (Sint16) rect->y;
		srcRect.w = (Uint16) rect->width;
		srcRect.h = (Uint16) rect->height;
		SDL_BlitSurface(sur, &srcRect, static_cast<SDLVideoSystem*>(const_cast<VideoSystem*>(screen))->m_sdlVideoSystem, &destRect);
	}
	else
	{
		SDL_BlitSurface(sur, NULL, static_cast<SDLVideoSystem*>(const_cast<VideoSystem*>(screen))->m_sdlVideoSystem, &destRect);
	}
	SDL_FreeSurface(sur);
}

int SDLFont::getSingleLineTextWidth(const std::string& text, Size size) const
{
	int width = 0;
	TTF_SizeText(ttfFont[size], text.c_str(), &width, NULL);

	return width;
}

int SDLFont::getTextWidth(const std::string& text, Size size) const
{
	if(!ttfFont[size])
	{
		return -1;
	}

	int maxWidth = 0;
	int width = 0;
	std::string::size_type oldPos = 0;
	std::string::size_type pos = text.find("\n");

	while(pos != std::string::npos)
	{
		width = getSingleLineTextWidth(text.substr(oldPos, pos-oldPos).c_str(), size);
		if(width > maxWidth)
		{
			maxWidth = width;
		}
		oldPos = pos + 1;
		pos = text.find("\n", oldPos);
	}
	if(oldPos < text.size())
	{
		width = getSingleLineTextWidth(text.substr(oldPos).c_str(), size);
	}
	if(width > maxWidth)
	{
		maxWidth = width;
	}
	return maxWidth;
}

int SDLFont::getTextHeight(const std::string& text, Size size) const
{
	if(!ttfFont[size])
	{
		return -1;
	}

	if(!text.size())
	{
		return 0;
	}

	int lines = 1;
	std::string::size_type pos = text.find("\n");
	while(pos != std::string::npos)
	{
		++lines;
		pos = text.find("\n", pos+1);
	}
	return lines * TTF_FontLineSkip(ttfFont[size]);
}

bool SDLFont::load()
{
	if(m_isLoaded)
	{
		return true;
	}

	for(int i = SIZE_COUNT - 1;i >= 0; --i)
	{
		SDL_RWops* fileOps = getRWops();
		if(!fileOps)
		{
			std::cout << "Couldn't load TTF-font file: " << TTF_GetError() << std::endl;
			return false;
		}
		ttfFont[i] = TTF_OpenFontRW(fileOps, 1, m_fontSizes[i]);

		if(!ttfFont[i])
		{
			std::cout << "Couldn't load TTF-font file: " << TTF_GetError() << std::endl;
			return false;
		}
	}

	return (m_isLoaded = true);
}

void SDLFont::unload()
{
	if(m_isLoaded)
	{
		for(int i = SIZE_COUNT - 1;i >= 0; --i)
		{
			if(ttfFont[i])
			{
				TTF_CloseFont(ttfFont[i]);
			}
		}
		m_isLoaded = false;
	}
}
