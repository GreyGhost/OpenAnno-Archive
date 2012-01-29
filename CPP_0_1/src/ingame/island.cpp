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

#include "island.h"
#include <physfs.h>
#include "lib/tinyxml/tinyxml.h"
#include <sstream>
#include <iostream>
#include "exception.h"
#include "gui/widgetmanager.h"

Island::Island(const std::string& filename, int pos_x, int pos_y, std::vector<bool>& waterTiles) : m_x(pos_x), m_y(pos_y), m_tiles(NULL), m_defaultTile(0)
{
	m_defaultTile = Tile::getDefaultTile();
	std::cout << "Trying to load island \"" << filename << "\"" << std::endl;
	TiXmlDocument document;
	if (!document.PHYSFS_LoadFile(filename))
	{
		throw FileLoadException(filename, document.ErrorDesc());
	}

	if(!document.RootElement() || document.RootElement()->ValueStr() != "island")
		throw XMLException(filename, -1, "This is no valid island XML file (missing island root element)");

	TiXmlElement *island = document.RootElement();
	if (!island->Attribute("cols"))
		throw XMLException(filename, island->Row(), "Missing attribute cols");
	if (!island->Attribute("rows"))
		throw XMLException(filename, island->Row(), "Missing attribute rows");
	if (!island->Attribute("clime"))
		throw XMLException(filename, island->Row(), "Missing attribute clime");
	std::stringstream attr;
	attr << island->Attribute("cols") << " " << island->Attribute("rows");
	attr >> m_cols >> m_rows;
	m_clime = island->Attribute("clime");
	std::cout << "Creating " << (m_rows * m_cols) << " tiles" << std::endl;
	m_tiles = new Tile* [m_rows * m_cols];
	memset(m_tiles, 0, sizeof(Tile*) * m_rows * m_cols);

	TiXmlElement *terrain = island->FirstChildElement("terrain");
	if (!terrain)
		throw XMLException(filename, island->Row(), "Missing toplevel element terrain");

	TiXmlElement *row = terrain->FirstChildElement("row");
	if (!terrain)
		throw XMLException(filename, terrain->Row(), "Missing row subelements");
	while (row)
	{
		if (!row->Attribute("value"))
			throw XMLException(filename, row->Row(), "Missing attribute value");
		std::stringstream rowStr(row->Attribute("value"));
		int rowInt;
		rowStr >> rowInt;
		rowInt--;
		TiXmlElement *col = row->FirstChildElement("col");
		if (!col)
			throw XMLException(filename, row->Row(), "Missing col subelements");
		while (col)
		{
			if (!col->Attribute("value"))
				throw XMLException(filename, col->Row(), "Missing attribute value");
			std::stringstream colStr(col->Attribute("value"));
			int colInt;
			colStr >> colInt;
			colInt--;

			TiXmlElement *tile = col->FirstChildElement("tile");
			if (!tile)
				throw XMLException(filename, col->Row(), "Missing tile subelement");
			if (((rowInt * m_cols) + colInt) >= m_cols * m_rows)
				std::cout << "WARNING! Index out of bounds. Row: " << rowInt << ", column: " << colInt << std::endl;
			else
			{
				m_tiles[(rowInt * m_cols) + colInt] = new Tile(tile);
				if (m_tiles[(rowInt * m_cols) + colInt] == NULL) {
					std::cout << "TILE CREATION FAILED" << std::endl;
				}
				waterTiles[(rowInt * m_cols) + colInt] = false;
			}

			col = col->NextSiblingElement("col");
		}
		row = row->NextSiblingElement("row");
	}

	std::cout << "Succesfully loaded island \"" << filename << "\"" << std::endl;
	std::cout << "\tColums: " << m_cols << std::endl;
	std::cout << "\tRows: " << m_rows << std::endl;
/*	std::cout << "debug-listing 0,0 to 9,9" << std::endl;
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			std::cout << m_tiles[(y * m_cols) + x]->getName();
			std::cout << ",";
		}
		std::cout << std::endl;
	}*/
}

Island::~Island()
{
	if (m_tiles)
	{
		std::cout << "Destroying " << (m_rows * m_cols) << " tiles" << std::endl;
		for (int i = 0; i < m_rows * m_cols; i++)
		{
			delete m_tiles[i];
		}
		delete [] m_tiles;
	}
}

bool Island::hasTileAt(int col, int row) const
{
	int c = col - m_x, r = row - m_y;
	return (c >= 0 && c < m_cols && r >= 0 && r < m_rows && m_tiles[c+r*m_cols]);
}

void Island::drawTile(int col, int row, int x, int y, Rotation direction, float zoom, float time) const
{
	int c = col - m_x, r = row - m_y;
	m_tiles[c+r*m_cols]->draw(direction, x, y, zoom, time, VideoSystem::getInstance());
}
