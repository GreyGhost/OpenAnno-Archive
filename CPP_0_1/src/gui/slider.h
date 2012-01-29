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

#ifndef SLIDER_H
#define SLIDER_H

#include <string>
#include "video/videosystem.h"
#include "video/surface.h"
#include "gui/commandwidget.h"
#include "gui/events/eventmouseclick.h"
#include "gui/events/eventmousemove.h"
#include <sstream>

class Slider : public CommandWidget
{
public:
	Slider(int x, int y, int min, int max, int commandId = -1);
	virtual ~Slider();
	void draw(const VideoSystem* const screen);
	void mouseClick(EventMouseClick* event);
	void mouseMove(EventMouseMove* event);
	void setPos(int pos);
	int getPos() { return m_pos; };
	bool isPointInWidget(int x, int y);
	std::string getTooltipText() const;

protected:
	int m_min, m_max;
	int m_pos;
	int m_dragButtonPos;
	SurfacePtr m_dragButton;
	bool m_doDrag;
	int m_dragButtonOffsetY;
};


#endif
