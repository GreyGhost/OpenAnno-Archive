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
#include "spriteanimation.h"
#include "video/videosystem.h"


SpriteAnimation::SpriteAnimation(TiXmlNode* node) : m_currentFrame(0), m_delay(0)
{
	if(!node || node->ValueStr() != "animation")
	{
		std::cout << "Invalid Sprite Format" << std::endl;
		return;
	}
	TiXmlElement* frame = node->FirstChildElement("frame");
	SurfacePtr surface;
	int delay = 0;
	while(frame)
	{
		TiXmlElement* temp = frame->FirstChildElement("surface");
		if(temp && temp->FirstChild())
		{
			surface = VideoSystem::getInstance()->createSurface(temp->FirstChild()->Value());
			TiXmlElement* temp = frame->FirstChildElement("delay");
			if(temp && temp->FirstChild())
			{
				std::istringstream stream(temp->FirstChild()->Value());
				stream >> delay;
				m_frames.push_back(surface);
				m_frameDelays.push_back(delay);
			}
		}
		frame = frame->NextSiblingElement("frame");
	}
}

SpriteAnimation::~SpriteAnimation()
{
	m_frames.clear();
}

void SpriteAnimation::update(int delay)
{
	if(m_frameDelays[m_currentFrame] == 0)
		return;

	m_delay += delay;
	while(m_delay > m_frameDelays[m_currentFrame])
	{
		++m_currentFrame;
		if(m_currentFrame >= m_frames.size())
		{
			m_currentFrame = 0;
		}
		m_delay -= m_frameDelays[m_currentFrame];
	}
}

void SpriteAnimation::draw(int x, int y) const
{
	if(m_frames[m_currentFrame].isNotNull())
	{
		VideoSystem::getInstance()->drawSurface(m_frames[m_currentFrame], x, y);
	}
}





