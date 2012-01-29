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

#ifndef FONT_H
#define FONT_H

#include <string>
#include "video/color.h"
#include "rect.h"
#include "io/resource.h"
#include "io/refptr.h"

class VideoSystem;


class Font : public Resource
{
public:
	enum Size
	{
		SIZE_SMALL,
		SIZE_MIDDLE,
		SIZE_BIG,
		SIZE_COUNT
	};
	Font(const std::string& filename) : Resource(filename, Resource::FONT, true) { m_fontSizes[SIZE_SMALL] = 17; m_fontSizes[SIZE_MIDDLE] = 20; m_fontSizes[SIZE_BIG] = 23; };
	virtual ~Font() { };
	virtual void drawText(const VideoSystem* const screen, std::string text, int x, int y, const Color& color, Size size, const Rect* const rect = NULL) const = 0;
	virtual int getTextWidth(const std::string& text, Size size) const = 0;
	virtual int getTextHeight(const std::string& text, Size size) const = 0;
protected:
	int m_fontSizes[SIZE_COUNT];
};

typedef RefPtr<Font> FontPtr;

#endif
