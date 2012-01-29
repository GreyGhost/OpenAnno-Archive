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

#ifndef SDLFONT_H
#define SDLFONT_H

#ifdef _WIN32
# include <windows.h>
#endif
#ifdef _WIN32
# include <SDL_ttf.h>
#else
# include <SDL/SDL_ttf.h>
#endif
#include <string>
#include "video/font.h"

class VideoSystem;

class SDLFont : public Font
{
public:
	SDLFont(const std::string& filename);
	~SDLFont();
	void drawText(const VideoSystem* const screen, std::string text, int x, int y, const Color& color, Size size, const Rect* const rect = NULL) const;
	int getTextWidth(const std::string& text, Size size) const;
	int getTextHeight(const std::string& text, Size size) const;
	bool load();
	void unload();
private:
	int getSingleLineTextWidth(const std::string& text, Size SIZE_COUNT) const;
	SDL_Surface* m_surface;
	TTF_Font* ttfFont[SIZE_COUNT];
};


#endif
