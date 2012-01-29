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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <map>
#include "lib/tinyxml/tinyxml.h"
#include "physfs.h"

class Config
{
public:
	~Config();
	static bool load(const std::string& configfilename);
	static bool save(const std::string& configfilename);
	static std::string getString(const std::string& option);
	static float getFloat(const std::string& option);
	static int getInt(const std::string& option);
	static void setString(const std::string& option, const std::string& value);
	static void setFloat(const std::string& option, float Value);
	static void setInt(const std::string& option, int value);

private:
	Config();
	TiXmlDocument m_xmlDocument;
	static Config m_instance;
	std::map<std::string, std::string> m_stringCache;
	std::map<std::string, float> m_floatCache;
	std::map<std::string, int> m_intCache;
};

#endif
