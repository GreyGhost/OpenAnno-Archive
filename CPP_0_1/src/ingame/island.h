/***************************************************************************
 *   Copyright (C) 2006 by Sven Rech                                       *
 *   svenrech@gmx.de                                                       *
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

#ifndef ISLAND_H
#define ISLAND_H

#include "tile.h"
#include <string>
#include <vector>
class Island
{
public:
	Island(const std::string& filename, int pos_x, int pos_y, std::vector<bool>& waterTiles); 
	~Island();
	void set_pos(int x, int y) { m_x = x; m_y = y; }
	int get_posX() { return m_x; }
	int get_posY() { return m_y; }
	int get_width() { return m_rows; }
	int get_height() { return m_cols; }
	void drawTile(int col, int row, int x, int y, Rotation direction, float zoom, float time) const;
	void draw(int scrollOffsetX, int scrollOffsetY, Rotation direction, int tileWidth, int halfTileHeight, int width, int height, float zoom, float time);
	bool hasTileAt(int col, int row) const;

private:
	Island(const Island&) : m_defaultTile(0)
	{}
	Island operator= (Island b)
	{}
	int m_x;
	int m_y;
	int m_rows;
	int m_cols;
	std::string m_clime;
	Tile** m_tiles;
	Tile m_defaultTile;
};

#endif
