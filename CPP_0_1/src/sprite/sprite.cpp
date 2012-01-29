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

#include "sprite.h"

Sprite::Sprite() : m_currentAnimation(NULL)
{

}

Sprite::~Sprite()
{
	for(std::map<std::string, SpriteAnimation*>::iterator it = m_animations.begin(); it != m_animations.end();++it)
	{
		delete it->second;
		it->second = NULL;
	}
	m_animations.clear();
}

bool Sprite::load(const std::string& filename)
{
	TiXmlDocument doc;
	if(!doc.PHYSFS_LoadFile(filename))
	{
		std::cout << "Couldn't load sprite \"" <<  filename << doc.Error() << std::endl;
		return false;
	}

	if(!doc.RootElement() || doc.RootElement()->ValueStr() != "sprite")
	{
		std::cout << "Couldn't load sprite \"" <<  filename << "\": Invalid XML-Format" << std::endl;
		return false;
	}
	TiXmlElement* animation = doc.RootElement()->FirstChildElement("animation");
	while(animation)
	{
		if(std::string(animation->Attribute("name")).size())
		{
			m_animations[animation->Attribute("name")] = new SpriteAnimation(animation);
		}
		else
		{
			std::cout << "\"" << filename << "\": <animation> missing name attribute" << std::endl;
		}
		animation = animation->NextSiblingElement("animation");
	}
	if(m_animations.size())
	{
		m_currentAnimation = m_animations.begin()->second;
	}
	return true;
}

void Sprite::update(int delay)
{
	if(m_currentAnimation)
	{
		m_currentAnimation->update(delay);
	}
}

void Sprite::draw(int x, int y) const
{
	if(m_currentAnimation)
	{
		m_currentAnimation->draw(x, y);
	}
}

bool Sprite::setAnimation(const std::string& animationName)
{
	std::map<std::string, SpriteAnimation*>::iterator it = m_animations.find(animationName);
	if(it != m_animations.end())
	{
		m_currentAnimation = it->second;
		return true;
	}
	return false;
}
