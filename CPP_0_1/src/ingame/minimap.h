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

#ifndef MINIMAP_H
#define MINIMAP_H

#include <string>
#include "gui/commandwidget.h"
#include "video/videosystem.h"
#include "map.h"

#include "gui/widgetmanager.h"
#include "gui/events/eventmouseout.h"
#include "gui/events/eventmousemove.h"
#include "gui/events/eventmouseclick.h"
#include "gui/events/eventtick.h"

class MiniMap : public CommandWidget
{
public:
	MiniMap(int width, int height, Map* map, int commandId);
	~MiniMap();
	void draw(const VideoSystem* const screen);

private:
	void onMouseOut(EventMouseOut* event);
	void onMouseMove(EventMouseMove* event);
	void onMouseClick(EventMouseClick* event);
	void update(EventTick* event);

	bool m_dragging;

	SurfacePtr m_hudBorder;
	Map* m_map;
};

#endif
