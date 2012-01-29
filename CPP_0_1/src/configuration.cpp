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

#include "configuration.h"
#include <sstream>

Config Config::m_instance;

Config::Config()
{
	m_xmlDocument.InsertEndChild(TiXmlDeclaration("1.0", "UTF-8", ""));
	m_xmlDocument.InsertEndChild(TiXmlElement("config"));
}

Config::~Config()
{
}

bool Config::load(const std::string& configfilename)
{
	if (!m_instance.m_xmlDocument.PHYSFS_LoadFile(configfilename))
		return false;

	if(!m_instance.m_xmlDocument.RootElement() || m_instance.m_xmlDocument.RootElement()->ValueStr() != "config")
	{
		m_instance.m_xmlDocument.RemoveChild(m_instance.m_xmlDocument.RootElement());
		m_instance.m_xmlDocument.InsertEndChild(TiXmlElement("config"));
		return false;
	}

	return true;
}

bool Config::save(const std::string& configfilename)
{
	return m_instance.m_xmlDocument.PHYSFS_SaveFile(configfilename.c_str());
}

std::string Config::getString(const std::string& option)
{
	if (m_instance.m_stringCache.find(option) != m_instance.m_stringCache.end())
		return m_instance.m_stringCache[option];

	if(!m_instance.m_xmlDocument.RootElement())
		return "";

	TiXmlElement* element = m_instance.m_xmlDocument.RootElement()->FirstChildElement(option);

	if(element)
	{
		std::string ret = element->FirstChild()->ValueStr();
		m_instance.m_stringCache[option] = ret;
		return ret;
	}

	return "";
}

float Config::getFloat(const std::string& option)
{
	if (m_instance.m_floatCache.find(option) != m_instance.m_floatCache.end())
		return m_instance.m_floatCache[option];

	if(!m_instance.m_xmlDocument.RootElement())
		return 0.0f;

	TiXmlElement* element = m_instance.m_xmlDocument.RootElement()->FirstChildElement(option);

	if(element && element->FirstChild())
	{
		float f;
		std::istringstream stream(element->FirstChild()->ValueStr());
		stream >> f;
		m_instance.m_floatCache[option] = f;
		return f;
	}

	return 0.0f;
}

int Config::getInt(const std::string& option)
{
	if (m_instance.m_intCache.find(option) != m_instance.m_intCache.end())
		return m_instance.m_intCache[option];

	if(!m_instance.m_xmlDocument.RootElement())
		return 0;

	TiXmlElement* element = m_instance.m_xmlDocument.RootElement()->FirstChildElement(option);

	if(element && element->FirstChild())
	{
		int i;
		std::istringstream stream(element->FirstChild()->ValueStr());
		stream >> i;
		m_instance.m_intCache[option] = i;
		return i;
	}

	return 0;
}

void Config::setString(const std::string& option, const std::string& value)
{
	m_instance.m_stringCache[option] = value;
	TiXmlElement* element = m_instance.m_xmlDocument.RootElement()->FirstChildElement(option);

	if(element)
	{
		if(element->FirstChild())
		{
			element->FirstChild()->SetValue(value);
		}
		else
		{
			element->InsertEndChild(TiXmlText(value));
		}
	}
}

void Config::setFloat(const std::string& option, float value)
{
	m_instance.m_floatCache[option] = value;
	TiXmlElement* element = m_instance.m_xmlDocument.RootElement()->FirstChildElement(option);

	if(element)
	{
		std::ostringstream stream;
		stream << value;
		if(element->FirstChild())
		{
			element->FirstChild()->SetValue(stream.str());
		}
		else
		{
			element->InsertEndChild(TiXmlText(stream.str()));
		}
	}
}

void Config::setInt(const std::string& option, int value)
{
	m_instance.m_intCache[option] = value;
	TiXmlElement* element = m_instance.m_xmlDocument.RootElement()->FirstChildElement(option);

	if(element)
	{
		std::ostringstream stream;
		stream << value;
		if(element->FirstChild())
		{
			element->FirstChild()->SetValue(stream.str());
		}
		else
		{
			element->InsertEndChild(TiXmlText(stream.str()));
		}
	}
}
