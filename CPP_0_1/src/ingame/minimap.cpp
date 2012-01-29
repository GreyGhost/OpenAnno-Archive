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

#include <sstream>
#include <math.h>
#include "minimap.h"
#include "map.h"
#include "video/videosystem.h"
#include "configuration.h"
#include "gui/imagebutton.h"

MiniMap::MiniMap(int width, int height, Map* map, int commandId) : CommandWidget(10, height - 160 + 10, commandId)
{
	m_width = 140;
	m_height = 140;
	m_map = map;
	
	m_dragging = false;
	
	m_hudBorder = VideoSystem::getInstance()->createSurface("images/hud/minimap.png");
	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<MiniMap, EventMouseOut>(this, &MiniMap::onMouseOut));
	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<MiniMap, EventMouseMove>(this, &MiniMap::onMouseMove));
	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<MiniMap, EventMouseClick>(this, &MiniMap::onMouseClick));
	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<MiniMap, EventTick>(this, &MiniMap::update));
}

MiniMap::~MiniMap()
{
}

void MiniMap::draw(const VideoSystem* const screen)
{
	Widget::draw(screen);
	screen->drawSurface(m_hudBorder, 0, VideoSystem::getInstance()->getHeight() - 192);
	screen->drawRect(10, VideoSystem::getInstance()->getHeight() - 160 + 10, 140, 140, Color::BLACK);
}

void MiniMap::update(EventTick* event)
{
	//some things need to be done here
	return;
}

void MiniMap::onMouseOut(EventMouseOut* event)
{
	(void)event;
	m_dragging = false;
}

void MiniMap::onMouseMove(EventMouseMove* event)
{
	if (m_dragging)
	{
		m_map->jump(float(event->m_absX - 10) / 140.0, float(event->m_absY - VideoSystem::getInstance()->getHeight() + 150) / 140.0);
	}
}

void MiniMap::onMouseClick(EventMouseClick* event)
{
	switch(event->m_button)
	{
		case EventMouseClick::BTN_LEFT:
			m_dragging = event->m_isButtonDown;
		break;
		case EventMouseClick::BTN_WHEELUP:
		case EventMouseClick::BTN_WHEELDOWN:
			break; //do something usefull
	}
}
