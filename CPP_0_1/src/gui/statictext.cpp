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

#include "statictext.h"
#include "widgetmanager.h"

StaticText::StaticText(int x, int y, const std::string& caption) : Widget(x, y), m_caption(caption)
{
	m_width = VideoSystem::getInstance()->getDefaultFont()->getTextWidth(caption, Font::SIZE_SMALL);
	m_height = VideoSystem::getInstance()->getDefaultFont()->getTextHeight(caption, Font::SIZE_SMALL);
}

StaticText::~StaticText()
{
}

void StaticText::draw(const VideoSystem* const screen)
{
	screen->drawText(m_caption, m_absX, m_absY, Color::BLACK, Font::SIZE_SMALL);

	Widget::draw(screen);
}

std::string StaticText::getCaption()
{
	return m_caption;
}

void StaticText::setCaption(const std::string& caption)
{
	m_caption = caption;
	m_width = VideoSystem::getInstance()->getDefaultFont()->getTextWidth(m_caption, Font::SIZE_SMALL);
	m_height = VideoSystem::getInstance()->getDefaultFont()->getTextHeight(m_caption, Font::SIZE_SMALL);
}
