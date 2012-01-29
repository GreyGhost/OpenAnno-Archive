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

#include "mission.h"
#include "lib/tinyxml/tinyxml.h"
#include <sstream>
#include <iostream>
#include "exception.h"

Mission::Mission(Map* map, MiniMap* miniMap) : m_map(map), m_miniMap(miniMap)
{
}

void Mission::load(const std::string& filename)
{
	std::cout << "Trying to load mission \"" << filename << "\"" << std::endl;
	TiXmlDocument document;
	if (!document.PHYSFS_LoadFile(filename))
	{
		throw FileLoadException(filename, document.ErrorDesc());
	}

	if(!document.RootElement() || document.RootElement()->ValueStr() != "mission")
	{
		throw XMLException(filename, -1, "This is no valid mission XML file (missing mission root element)");
	}

	TiXmlElement *mission = document.RootElement();

	//User Informations:
	{
		TiXmlElement *info = mission->FirstChildElement("info");
		if (!info)
			throw XMLException(filename, mission->Row(), "Missing toplevel element info");
		TiXmlElement* tlevel = info->FirstChildElement("level");
		if (!tlevel)
			throw XMLException(filename, info->Row(), "Missing level subelement of info");
		TiXmlNode* levelchild = tlevel->FirstChild();
		if (!levelchild || !levelchild->Value())
			throw XMLException(filename, tlevel->Row(), "Missing text in level subelement of info");
		std::stringstream level(levelchild->Value());
		level >> m_level;
		TiXmlElement* name = info->FirstChildElement("name");
		if (!name)
			throw XMLException(filename, info->Row(), "Missing name subelement of info");
		TiXmlNode* namechild = name->FirstChild();
		if (!namechild || !namechild->Value())
			throw XMLException(filename, name->Row(), "Missing text in name subelement of info");
		m_name = namechild->Value();
		TiXmlElement* description = info->FirstChildElement("description");
		if (!description)
			throw XMLException(filename, info->Row(), "Missing description subelement of info");
		TiXmlNode* descriptionchild = description->FirstChild();
		if (!descriptionchild || !descriptionchild->Value())
			throw XMLException(filename, description->Row(),  "Mtext in description subelement of info");
		m_description = descriptionchild->Value();
	}

	//Resources:
	{
		TiXmlElement *resources = mission->FirstChildElement("resources");
		std::stringstream res;
		res << resources->FirstChildElement("gold")->FirstChild()->Value() << " " 
		    << resources->FirstChildElement("wood")->FirstChild()->Value() << " "
		    << resources->FirstChildElement("stone")->FirstChild()->Value() << " "
		    << resources->FirstChildElement("tools")->FirstChild()->Value() << " "
		    << resources->FirstChildElement("food")->FirstChild()->Value()  << " "
		    << resources->FirstChildElement("clothes")->FirstChild()->Value()  << " "
		    << resources->FirstChildElement("wool")->FirstChild()->Value()  << " "
		    << resources->FirstChildElement("tobacco")->FirstChild()->Value();
		res >> m_gold >> m_wood >> m_stone >> m_tools >> m_food >> m_clothes >> m_wool >> m_tobacco;
	}

	//Misc:
	{
		TiXmlElement* misc = mission->FirstChildElement("misc");
		std::stringstream miscStream;
		miscStream << misc->FirstChildElement("drouth")->FirstChild()->Value() << " "
		     << misc->FirstChildElement("plague")->FirstChild()->Value() << " "
		     << misc->FirstChildElement("pirates")->FirstChild()->Value() << " "
		     << misc->FirstChildElement("trader")->FirstChild()->Value();
		miscStream >> m_drouth >> m_plague >> m_pirates >> m_trader;
	}

	//Map:
	if (m_map != NULL)
		m_map->load(mission->FirstChildElement("map"));

	//Objective:
	{
		TiXmlElement* objective = mission->FirstChildElement("objective");
		std::stringstream obj;
		obj << objective->FirstChildElement("towns")->FirstChild()->Value() << " "
		    << objective->FirstChildElement("inhabitants")->FirstChild()->Value() << " "
		    << objective->FirstChildElement("society")->FirstChild()->Value();
		obj >> m_towns >> m_inhabitants >> m_society;
	}

	std::cout << "Succesfully loaded mission \"" << filename << "\"" << std::endl;
}

bool Mission::objectCompleted()
{
	return false;
}
