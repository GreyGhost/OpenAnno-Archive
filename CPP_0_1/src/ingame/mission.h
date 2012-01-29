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

#ifndef MISSION_H
#define MISSION_H

#include <string>
#include "map.h"
#include "minimap.h"

class Mission 
{
public:
	Mission(Map* map, MiniMap* miniMap);
	void load(const std::string & filename);
	bool objectCompleted();
private:
	Map* m_map;
	MiniMap* m_miniMap;

	int m_level;
	std::string m_name;
	std::string m_description;

	int m_gold;
	int m_wood;
	int m_stone;
	int m_tools;
	int m_food;
	int m_clothes;
	int m_wool;
	int m_tobacco;

	int m_drouth;
	int m_plague;
	int m_pirates;
	int m_trader;

	int m_towns;
	int m_inhabitants;
	int m_society;
};

#endif
