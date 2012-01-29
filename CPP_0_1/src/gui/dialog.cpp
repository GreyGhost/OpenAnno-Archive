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

#include "dialog.h"

Dialog::Dialog(int x, int y, int width, int height, const std::string& background, int commandId) : CommandWidget(x, y, commandId)
{
	m_background = VideoSystem::getInstance()->createSurface(background);
	if(width == -1 && height == -1)
	{
		if(m_background.isNotNull())
		{
			m_width = m_background->getWidth();
			m_height = m_background->getHeight();
		}
		else
		{
			m_width = 320;
			m_height = 240;
		}
	}
	else
	{
		m_width = width;
		m_height = height;
	}
}

Dialog::~Dialog()
{
}

void Dialog::draw(const VideoSystem* const screen)
{
	if(m_background.isNotNull())
	{
		VideoSystem::getInstance()->drawSurface(m_background, m_absX, m_absY);
	}
	else
	{
		VideoSystem::getInstance()->drawFilledRect(m_absX, m_absY, m_width, m_height, Color::GRAY40);
		VideoSystem::getInstance()->drawRect(m_absX, m_absY, m_width, m_height, Color::BLACK);
	}
	Widget::draw(screen);
}
