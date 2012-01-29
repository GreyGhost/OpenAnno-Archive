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
#include "map.h"
#include "lib/tinyxml/tinyxml.h"
#include "video/videosystem.h"
#include "configuration.h"

Map::Map(int x, int y, int width, int height) : Widget(x, y), m_cols(0), m_rows(0), m_direction(Rotation::NORTH), m_scrollOffsetX(0), m_scrollOffsetY(0), m_zoom(1.0), m_tileWidth(Map::tileWidth), m_halfTileHeight(Map::halfTileHeight), m_defaultTile(0), m_isShifting(false)
{
	m_width = width;
	m_height = height;
	m_time = 0.0f;

	std::cout << "Screen width: " << width << ", height: " << height << std::endl;

	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<Map, EventMouseMove>(this, &Map::onMouseMove));
	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<Map, EventMouseClick>(this, &Map::onMouseClick));
	m_defaultTile = Tile::getDefaultTile();
}

Map::~Map()
{
	for (std::vector<Island*>::iterator i = m_islands.begin(); i != m_islands.end(); i++)
	{
		assert(*i);
		delete (*i);
	}
}

bool Map::load(TiXmlElement* elmMap)
{
	std::cout << "Loading map..." << std::endl;
	std::stringstream dummy;
	dummy << elmMap->FirstChildElement("width")->FirstChild()->Value() << " "
	      << elmMap->FirstChildElement("height")->FirstChild()->Value() << " ";
	dummy >> m_cols >> m_rows;
	std::cout << " Dimensions: (" << m_cols << "x" << m_rows << ")" << std::endl;

	m_waterTiles.clear();
	m_waterTiles.resize(m_cols*m_rows, true);

	TiXmlElement* startpos = elmMap->FirstChildElement("startposition");
	startpos->Attribute("x", &m_scrollOffsetX);
	startpos->Attribute("y", &m_scrollOffsetY);

	std::string orientation = elmMap->FirstChildElement("orientation")->FirstChild()->Value();

	TiXmlElement* elmIsland = elmMap->FirstChildElement("island");
	// I suppose the manual definition (width and height in the toplevel element) supersed this
/*	int width = 0;
	int height = 0;*/
	while(elmIsland)
	{
		std::cout << " Adding island..." << std::endl;
		int islandX, islandY;
		elmIsland->Attribute("start_col", &islandX);
		elmIsland->Attribute("start_row", &islandY);
		m_islands.push_back(new Island("xml/islands/" + elmIsland->FirstChild()->ValueStr(), islandX, islandY, m_waterTiles));
/*		int tmpwidth = islandX + m_islands.at(m_islands.size() - 1)->get_width();
		if (tmpwidth > width)
			width = tmpwidth;
		int tmpheight = islandY + m_islands.at(m_islands.size() - 1)->get_height();
		if (tmpheight > height)
			height = tmpheight;*/
		//TODO
		std::cout << " Done." << std::endl;
		elmIsland = elmIsland->NextSiblingElement("island");
	}
	/*m_cols = width;
	m_rows = height;*/
	/*int mostTop=0, mostBottom=m_cols+m_rows, mostLeft=0, mostRight=m_cols+m_rows;
	for(int i=m_cols*m_rows-1;i>=0;i--)
	{
		if(m_waterTiles[i])
		{
			int x=i%m_cols, y=i/m_cols;
			int top = x+y,left=m_rows+x-y;
			if(top<mostTop)
				mostTop=top;
			if(top>mostBottom)
				mostBottom=top;
			if(left<mostLeft)
				mostLeft=left;
			if(left>mostRight)
				mostRight=left;
		}
	}
	
	std::cout << " real Height: " << mostBottom-mostTop << " Width: " << mostRight-mostLeft << std::endl;*/
	
	std::cout << "Done." << std::endl;
	return true;
}
inline
unsigned int negmod(int val, int mod)
{
	if(val<0)
		return (val % mod) + mod;
	else
		return val % mod;
}
void Map::draw(const VideoSystem* const screen)
{
	bool v1 = negmod(m_scrollOffsetY,(m_halfTileHeight*2)) >= m_halfTileHeight, v2 = negmod(m_scrollOffsetX, m_tileWidth) >= m_tileWidth/2;
	int a=m_scrollOffsetY/(m_halfTileHeight*2);
	int b=m_scrollOffsetX/m_tileWidth;
	int col=a-b, row=a+b;
	int state=v1==v2?1:0;//0: rechts / 1: tief
	
	int xStart = -negmod(m_scrollOffsetX,m_tileWidth);
	int x=xStart-state*(m_tileWidth/2), y=-negmod(m_scrollOffsetY,m_halfTileHeight)-state*(m_halfTileHeight*2);
	
	if(v2)
	{
		y+=-m_halfTileHeight;
		col+=v1?-1:1;
		if(v1)
		{
			col-=1;
			row-=1;
		}
	}
	if(v1)
	{
		col+=2;
		row+=1;
	}
	//std::ostringstream tmp,tmp2;
	//tmp << "x: " << x*10/m_tileWidth << " y: " << y*5/m_halfTileHeight << " v1: " << v1 << " v2: " << v2 << " col: " << col << " row: " << row << " a: " << a << " b: " << b;
	while(y<m_height+200) //max image height
	{
		int count = 0;
		while(x<m_width)
		{
			if(col>=0 && row>=0 && col<m_cols && row<m_rows)
			{
				for(std::vector<Island*>::iterator i = m_islands.begin(); i != m_islands.end(); i++)
				{
					if((*i)->hasTileAt(col, row))
					{
						(*i)->drawTile(col, row, x, y, m_direction, m_zoom, m_time);
						goto foundTile;
					}
				}
			}
			//only executed if no Tile found
			m_defaultTile.draw(m_direction, x, y, m_zoom, m_time, screen);
			/*tmp2.clear();
			tmp2.str("");
			tmp2 << row << "  " << col;
			screen->drawText(tmp2.str(), x+m_tileWidth/3, y-m_halfTileHeight*3/2, Color::WHITE, Font::SIZE_SMALL);*/
			//else go here
			foundTile:
			
			x+=m_tileWidth;
			row+=1;
			col-=1;
			count++;
		}
		state=1&~state;
		col+=count;
		row-=count;
		x=xStart;
		if(state==1) //tief
		{
			row+=1;
		}
		else
		{
			col+=1;
			x-=m_tileWidth/2;
		}
		
		y+=m_halfTileHeight;
	}	
	//screen->drawText(tmp.str(), 5, 5, Color::WHITE, Font::SIZE_SMALL);

	Widget::draw(screen);
}

void Map::scroll(int offsetX, int offsetY)
{
	int
		m_minScrollOffsetX = -(m_rows-1)*(m_tileWidth/2),
		m_maxScrollOffsetX = (m_cols-1)*(m_tileWidth/2),
		m_minScrollOffsetY = 0,
		m_maxScrollOffsetY = (m_cols+m_rows-1)*m_halfTileHeight;

	m_scrollOffsetX += offsetX;
	if(m_scrollOffsetX < m_minScrollOffsetX)
		m_scrollOffsetX = m_minScrollOffsetX;
	else if(m_scrollOffsetX > m_maxScrollOffsetX)
		m_scrollOffsetX = m_maxScrollOffsetX;
	
	m_scrollOffsetY += offsetY;
	if(m_scrollOffsetY < m_minScrollOffsetY)
		m_scrollOffsetY = m_minScrollOffsetY;
	else if(m_scrollOffsetY > m_maxScrollOffsetY)
		m_scrollOffsetY = m_maxScrollOffsetY;

/*	if((m_direction == Rotation::NORTH || m_direction == Rotation::SOUTH) && m_scrollOffsetX > m_tileWidth * m_cols - m_width - m_tileWidth / 2)
	{
		m_scrollOffsetX = m_tileWidth * m_cols - m_width - m_tileWidth / 2;
	}
	else if((m_direction == Rotation::EAST || m_direction == Rotation::WEST) && m_scrollOffsetX > m_tileWidth * m_rows / 2 - m_width - m_tileWidth / 2)
	{
		m_scrollOffsetX = m_tileWidth * m_rows / 2 - m_width - m_tileWidth / 2;
	}
	if(m_scrollOffsetX < 0)
	{
		m_scrollOffsetX = 0;
	}

	if((m_direction == Rotation::NORTH || m_direction == Rotation::SOUTH) && m_scrollOffsetY > m_halfTileHeight * m_rows - m_height - m_halfTileHeight * 2)
	{
		m_scrollOffsetY = m_halfTileHeight * m_rows - m_height - m_halfTileHeight * 2;
	}
	else if((m_direction == Rotation::EAST || m_direction == Rotation::WEST) && m_scrollOffsetY > 2 * m_halfTileHeight * m_cols - m_height - m_halfTileHeight)
	{
		m_scrollOffsetY = 2 * m_halfTileHeight * m_cols - m_height - m_halfTileHeight;
	}
	if(m_scrollOffsetY < 0)
	{
		m_scrollOffsetY = 0;
	}*/
}

void Map::jump(float x, float y) {
	if(m_direction == Rotation::EAST)
	{
		m_scrollOffsetX = static_cast<int> ((y * (m_tileWidth * m_rows / 2 - m_tileWidth / 2) - m_width / 2) + 0.5f);
		m_scrollOffsetY = static_cast<int> (((1.0f - x) * (2 * m_halfTileHeight * m_cols - m_halfTileHeight) - m_height / 2) + 0.5f);
	}
	else if(m_direction == Rotation::SOUTH)
	{
		m_scrollOffsetX = static_cast<int> (((1.0f - x) * (m_tileWidth * m_cols - m_tileWidth / 2) - m_width / 2) + 0.5f);
		m_scrollOffsetY = static_cast<int> (((1.0f - y) * (m_halfTileHeight * m_rows - m_halfTileHeight) - m_height / 2) + 0.5f);
	}
	else if(m_direction == Rotation::WEST)
	{
		m_scrollOffsetX = static_cast<int> (((1.0f - y) * (m_tileWidth * m_rows / 2 - m_tileWidth / 2) - m_width / 2) + 0.5f);
		m_scrollOffsetY = static_cast<int> ((x * (2 * m_halfTileHeight * m_cols - m_halfTileHeight) - m_height / 2) + 0.5f);
	}
	else
	{
		m_direction = Rotation::NORTH;
		m_scrollOffsetX = static_cast<int> ((x * (m_tileWidth * m_cols - m_tileWidth / 2) - m_width / 2) + 0.5f);
		m_scrollOffsetY = static_cast<int> ((y * (m_halfTileHeight * m_rows - m_halfTileHeight) - m_height / 2) + 0.5f);
	}
	scroll(0, 0);
}

void Map::update(float delay)
{
	m_time += delay;
	//some things need to be done here
	return;
}

void Map::onMouseMove(EventMouseMove* event)
{
	m_mouseX = event->m_absX;
	m_mouseY = event->m_absY;

	if (m_isShifting)
	{
		scroll(-event->m_relX, -event->m_relY);
	}
}

void Map::onMouseClick(EventMouseClick* event)
{
	if(event->m_button == EventMouseClick::BTN_LEFT)
	{
		if(event->m_isButtonDown)
		{
			Uint8 *keystate = SDL_GetKeyState(NULL);
			if (keystate[SDLK_LSHIFT])
			{
				m_isShifting = true;
			}
		}
		else
		{
			m_isShifting = false;
		}
	}
	else if ((event->m_button == EventMouseClick::BTN_WHEELUP)||(event->m_button == EventMouseClick::BTN_WHEELDOWN))
	{
		const float zoomStep = 0.125f;

		switch (event->m_button)
		{
			case EventMouseClick::BTN_WHEELUP:
					zoom(zoomStep);
				break;
			case EventMouseClick::BTN_WHEELDOWN:
					zoom(-zoomStep);
				break;
		}
	}
}

void Map::zoom(float offsetZoom)
{
	const float maxZoom = 1.0f;
	const float minZoom = 0.125f;

	float oldZoom = m_zoom;
	m_zoom += offsetZoom;
	if(m_zoom < minZoom)
	{
		m_zoom = minZoom;
	}
	else if(m_zoom > maxZoom)
	{
		m_zoom = maxZoom;
	}

	m_halfTileHeight = static_cast<int>(Map::halfTileHeight * m_zoom);
	m_zoom = m_halfTileHeight / static_cast<float>(Map::halfTileHeight);
	m_tileWidth = static_cast<int>(Map::tileWidth * m_zoom);

	// adjust scrolloffset. we want to zoom centered so we have to add half width/height
	m_scrollOffsetX += m_width / 2;
	m_scrollOffsetY += m_height / 2;
	m_scrollOffsetX = static_cast<int>(m_scrollOffsetX * m_zoom / oldZoom);
	m_scrollOffsetY = static_cast<int>(m_scrollOffsetY * m_zoom / oldZoom);
	scroll(-m_width / 2, -m_height / 2);
	if(m_scrollOffsetX < 0) {
		m_scrollOffsetX = 0;
	}
	if(m_scrollOffsetY < 0) {
		m_scrollOffsetY = 0;
	}
}

void Map::rotateRight()
{
	int oldDiffX, newDiffX, oldDiffY, newDiffY;

	if(m_direction == Rotation::NORTH)
	{
		m_direction = Rotation::WEST;

		oldDiffY = m_halfTileHeight * m_rows - m_height - m_halfTileHeight;
		newDiffY = 2 * m_halfTileHeight * m_cols - m_height - m_halfTileHeight;
		oldDiffX = m_tileWidth * m_cols - m_width - m_tileWidth / 2;
		newDiffX = m_tileWidth * m_rows / 2 - m_width - m_tileWidth / 2;
	}
	else if(m_direction == Rotation::EAST)
	{
		m_direction = Rotation::NORTH;

		oldDiffY = 2 * m_halfTileHeight * m_cols - m_height - m_halfTileHeight;
		newDiffY = m_halfTileHeight * m_rows - m_height - m_halfTileHeight;
		oldDiffX = m_tileWidth * m_rows / 2 - m_width - m_tileWidth / 2;
		newDiffX = m_tileWidth * m_cols - m_width - m_tileWidth / 2;
	}
	else if(m_direction == Rotation::SOUTH)
	{
		m_direction = Rotation::EAST;

		oldDiffY = m_halfTileHeight * m_rows - m_height - m_halfTileHeight;
		newDiffY = 2 * m_halfTileHeight * m_cols - m_height - m_halfTileHeight;
		oldDiffX = m_tileWidth * m_cols - m_width - m_tileWidth / 2;
		newDiffX = m_tileWidth * m_rows / 2 - m_width - m_tileWidth / 2;
	}
	else if(m_direction == Rotation::WEST)
	{
		m_direction = Rotation::SOUTH;

		oldDiffY = 2 * m_halfTileHeight * m_cols - m_height - m_halfTileHeight;
		newDiffY = m_halfTileHeight * m_rows - m_height - m_halfTileHeight;
		oldDiffX = m_tileWidth * m_rows / 2 - m_width - m_tileWidth / 2;
		newDiffX = m_tileWidth * m_cols - m_width - m_tileWidth / 2;
	}
	else
	{
		m_direction = Rotation::NORTH;
		m_scrollOffsetX = m_scrollOffsetY = 0;
		return;
	}
	int tmp = newDiffX - m_scrollOffsetY * newDiffX / oldDiffY - (((m_scrollOffsetY * newDiffX * 2) % (oldDiffY * 2)) & 1);
	m_scrollOffsetY = m_scrollOffsetX * newDiffY / oldDiffX - (((m_scrollOffsetX * newDiffY * 2) % (oldDiffX * 2)) & 1);
	m_scrollOffsetX = tmp;
	scroll(0, 0);
}

void Map::rotateLeft()
{
	int oldDiffX, newDiffX, oldDiffY, newDiffY;

	if(m_direction == Rotation::NORTH)
	{
		m_direction = Rotation::EAST;

		oldDiffY = m_halfTileHeight * m_rows - m_height - m_halfTileHeight;
		newDiffY = 2 * m_halfTileHeight * m_cols - m_height - m_halfTileHeight;
		oldDiffX = m_tileWidth * m_cols - m_width - m_tileWidth / 2;
		newDiffX = m_tileWidth * m_rows / 2 - m_width - m_tileWidth / 2;
	}
	else if(m_direction == Rotation::EAST)
	{
		m_direction = Rotation::SOUTH;

		oldDiffY = 2 * m_halfTileHeight * m_cols - m_height - m_halfTileHeight;
		newDiffY = m_halfTileHeight * m_rows - m_height - m_halfTileHeight;
		oldDiffX = m_tileWidth * m_rows / 2 - m_width - m_tileWidth / 2;
		newDiffX = m_tileWidth * m_cols - m_width - m_tileWidth / 2;
	}
	else if(m_direction == Rotation::SOUTH)
	{
		m_direction = Rotation::WEST;

		oldDiffY = m_halfTileHeight * m_rows - m_height - m_halfTileHeight;
		newDiffY = 2 * m_halfTileHeight * m_cols - m_height - m_halfTileHeight;
		oldDiffX = m_tileWidth * m_cols - m_width - m_tileWidth / 2;
		newDiffX = m_tileWidth * m_rows / 2 - m_width - m_tileWidth / 2;
	}
	else if(m_direction == Rotation::WEST)
	{
		m_direction = Rotation::NORTH;

		oldDiffY = 2 * m_halfTileHeight * m_cols - m_height - m_halfTileHeight;
		newDiffY = m_halfTileHeight * m_rows - m_height - m_halfTileHeight;
		oldDiffX = m_tileWidth * m_rows / 2 - m_width - m_tileWidth / 2;
		newDiffX = m_tileWidth * m_cols - m_width - m_tileWidth / 2;
	}
	else
	{
		m_direction = Rotation::NORTH;
		m_scrollOffsetX = m_scrollOffsetY = 0;
		return;
	}
	int tmp = m_scrollOffsetY * newDiffX / oldDiffY + (((m_scrollOffsetY * newDiffX * 2) % (oldDiffY * 2)) & 1);
	m_scrollOffsetY = newDiffY - m_scrollOffsetX * newDiffY / oldDiffX - (((m_scrollOffsetX * newDiffY * 2) % (oldDiffX * 2)) & 1);
	m_scrollOffsetX = tmp;
	scroll(0, 0); //correct values out-of-range
}

/**
 * \todo Rewrite
 */
void Map::drawWater(int scrollOffsetX, int scrollOffsetY, Rotation direction, int tileWidth, int halfTileHeight, int width, int height, float zoom, float time)
{
	int toggle, tile, stdPosX[2], posX, posY, numX, numY, stdTileAddPerCol, stdTileAddPerRow[2];
	int insideBeginX[2], insideBeginY, insideEndX[2], insideEndY;
	stdPosX[1] = (stdPosX[0] = -(scrollOffsetX % tileWidth)) - tileWidth / 2;
	posY = -(scrollOffsetY % halfTileHeight) - halfTileHeight;
	if(direction == Rotation::NORTH)
	{
		tile = m_cols * (scrollOffsetY / halfTileHeight) + scrollOffsetX / tileWidth;
		numX = (width - stdPosX[1] + tileWidth - 1) / tileWidth;
		numY = (height - posY + halfTileHeight - 1) / halfTileHeight + 1;
		stdTileAddPerCol = 1;
		if (numX > m_cols)
			stdTileAddPerRow[0] = stdTileAddPerRow[1] = 0;
		else
			stdTileAddPerRow[0] = stdTileAddPerRow[1] = m_cols - numX;
		insideBeginX[0] = (numX - m_cols) / 2 + 1;
		insideBeginX[1] = (numX - m_cols) / 2;
		insideBeginY = (numY - m_rows) / 2;
		insideEndX[0] = insideBeginX[0] + m_cols;
		insideEndX[1] = insideBeginX[1] + m_cols;
		insideBeginX[0]++;
		insideBeginX[1]++;
		insideEndY = insideBeginY + m_rows;
		toggle = 1 & (tile / m_cols);
	}
	else if(direction == Rotation::WEST)
	{
		tile = (m_rows - 1) * m_cols - (scrollOffsetX / tileWidth) * 2 * m_cols + (scrollOffsetY / halfTileHeight / 2) -  (scrollOffsetY % (halfTileHeight * 2) >= halfTileHeight ? m_cols : 0);
		numX = (width - stdPosX[1] + tileWidth - 1) / tileWidth;
		numY = (height - posY + halfTileHeight - 1) / halfTileHeight + 1;
		stdTileAddPerCol = -2 * m_cols;
		if (numX > m_rows / 2)
		{
			stdTileAddPerRow[0] = 2 * (m_rows / 2) * m_cols - m_cols;
			stdTileAddPerRow[1] = m_cols + 2 * (m_rows / 2) * m_cols + 1;
		}
		else
		{
			stdTileAddPerRow[0] = 2 * numX * m_cols - m_cols;
			stdTileAddPerRow[1] = m_cols + 2 * numX * m_cols + 1;
		}
		insideBeginX[0] = insideBeginX[1] =  (numX - m_rows / 2) / 2 + 1;
		insideBeginY = (numY - m_cols * 2) / 2;
		insideEndX[0] = insideBeginX[0] + (m_rows / 2) - 1;
		insideEndX[1] = insideBeginX[1] + (m_rows / 2) - 1;
		insideEndY = insideBeginY + (m_cols * 2);
		toggle = 1 & (tile / m_cols + m_rows);
	}
	else if(direction == Rotation::SOUTH)
	{
		tile = m_cols * (m_rows - scrollOffsetY / halfTileHeight) - scrollOffsetX / tileWidth -1;
		numX = (width - stdPosX[1] + tileWidth - 1) / tileWidth;
		numY = (height - posY + halfTileHeight - 1) / halfTileHeight + 1;
		stdTileAddPerCol = -1;
		if (numX > m_cols)
			stdTileAddPerRow[0] = stdTileAddPerRow[1] = 0;
		else
			stdTileAddPerRow[0] = stdTileAddPerRow[1] = numX - m_cols;
		insideBeginX[0] = (numX - m_cols) / 2 + 1;
		insideBeginX[1] = (numX - m_cols) / 2;
		insideBeginY = (numY - m_rows) / 2;
		insideEndX[0] = insideBeginX[0] + m_cols;
		insideEndX[1] = insideBeginX[1] + m_cols;
		insideBeginX[0]++;
		insideBeginX[1]++;
		insideEndY = insideBeginY + m_rows;
		toggle = 1 & (tile / m_cols + m_rows + 1);
	}
	else if(direction == Rotation::EAST)
	{
		tile = m_cols - 1 + (scrollOffsetX / tileWidth) * 2 * m_cols - (scrollOffsetY / halfTileHeight / 2) + (scrollOffsetY % (halfTileHeight * 2) >= halfTileHeight ? m_cols : 0);
		numX = (width - stdPosX[1] + tileWidth - 1) / tileWidth;
		numY = (height - posY + halfTileHeight - 1) / halfTileHeight + 1;
		stdTileAddPerCol = 2 * m_cols;
		if (numX > m_rows / 2)
		{
			stdTileAddPerRow[0] = -2 * (m_rows / 2) * m_cols + m_cols;
			stdTileAddPerRow[1] = -2 * (m_rows / 2) * m_cols - m_cols - 1;
		}
		else
		{
			stdTileAddPerRow[0] = -2 * numX * m_cols + m_cols;
			stdTileAddPerRow[1] = -2 * numX * m_cols - m_cols - 1;
		}
		insideBeginX[0] = insideBeginX[1] =  (numX - m_rows / 2) / 2 + 1;
		insideBeginY = (numY - m_cols * 2) / 2;
		insideEndX[0] = insideBeginX[0] + (m_rows / 2) - 1;
		insideEndX[1] = insideBeginX[1] + (m_rows / 2) - 1;
		insideEndY = insideBeginY + (m_cols * 2);
		toggle = 1 & (tile / m_cols + 1);
	}
	posX = stdPosX[toggle];
	posY += 2*halfTileHeight;
	for(int i = numY; i > 0; --i)
	{
		bool insideMapY = true;
		if (numY > m_rows)
			if (i < insideBeginY + (toggle? 1 : 0) 
			   || i > insideEndY)
				insideMapY = false;
		for(int j = numX; j > 0; --j, posX += tileWidth)
		{
			bool insideMapX = true;
			if (numX > m_cols)
				if (j < insideBeginX[toggle] || j > insideEndX[toggle])
					insideMapX = false;

			bool draw = true;
			if (insideMapX && insideMapY)
				if (!m_waterTiles[tile])
					draw = false;

			if (draw)
				m_defaultTile.draw(direction, posX, posY, zoom, time, VideoSystem::getInstance());

			if (insideMapX && insideMapY)
				tile += stdTileAddPerCol;
		}
		toggle = 1 & ~toggle;
		if (insideMapY)
			tile += stdTileAddPerRow[toggle];
		posX = stdPosX[toggle];
		posY += halfTileHeight;
	}
}
