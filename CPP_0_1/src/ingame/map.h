/***************************************************************************
 *   Copyright (C) 2006 by Lars-Peter Clausen                              *
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

#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include "tile.h"
#include "island.h"
#include "gui/widget.h"
#include "video/videosystem.h"

#include "gui/widgetmanager.h"
#include "gui/events/eventmousemove.h"
#include "gui/events/eventmouseclick.h"

class Map : public Widget
{
	friend class MiniMap;
public:
	Map(int x, int y, int width, int height);
	~Map();
	bool load(TiXmlElement* map);
	void draw(const VideoSystem* const screen);
	void update(float delay);
	void scroll(int offsetX, int offsetY);
	void jump(float x, float y);
	void zoom(float offsetZoom);
	void rotateLeft();
	void rotateRight();
	Tile* getTile(int col, int row);

private:
	void onMouseMove(EventMouseMove* event);
	void onMouseClick(EventMouseClick* event);

	void drawWater(int scrollOffsetX, int scrollOffsetY, Rotation direction, int tileWidth, int halfTileHeight, int width, int height, float zoom, float time);

	int m_mouseX;
	int m_mouseY;

	int m_cols, m_rows;
	std::vector<Island*> m_islands;
	Rotation m_direction;
	Tile m_defaultTile;
	int m_scrollOffsetX, m_scrollOffsetY;
	float m_zoom;
	float m_time;

	static const int tileWidth = 128;
	static const int halfTileHeight = 32;

	int m_tileWidth; //the real tilesize (with zoom)
	int m_halfTileHeight;

	bool m_isShifting; // Scrolling with Shift + LMB

	std::vector<bool> m_waterTiles;
};

#endif
