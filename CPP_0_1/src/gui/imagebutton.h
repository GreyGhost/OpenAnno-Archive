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

#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include <string>
#include "video/videosystem.h"
#include "video/surface.h"
#include "gui/commandwidget.h"
#include "gui/events/eventmouseclick.h"
#include "gui/events/eventmouseover.h"
#include "gui/events/eventmouseout.h"

class ImageButton : public CommandWidget
{
public:
	ImageButton(int x, int y, const std::string& caption, const std::string& image, const std::string& imageHighlighted, int commandId = -1);
	~ImageButton();
	void draw(const VideoSystem* const screen);
	void onMouseClick(EventMouseClick* event);
	void onMouseOver(EventMouseOver* event);
	void onMouseOut(EventMouseOut* event);
private:
	SurfacePtr m_image;
	SurfacePtr m_imageHighlighted;
	std::string m_caption;
	int m_textOffsetY;
	bool m_isHighlighted;
};

#endif
