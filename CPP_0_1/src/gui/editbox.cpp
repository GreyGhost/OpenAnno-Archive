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

#include "editbox.h"
#include "widgetmanager.h"
#include "gui/events/eventkey.h"
#include <iostream>

Editbox::Editbox(int x, int y, int width, int commandId) : CommandWidget(x, y, commandId), m_text(""), m_cursorPos(2), m_cursorPosIndex(0), m_textOffsetX(0), m_hovered(false), m_timer(0.0f)
{
	m_width = width;
	m_height = VideoSystem::getInstance()->getDefaultFont()->getTextHeight(" ", Font::SIZE_SMALL) + 4;

	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<Editbox, EventKey>(this, &Editbox::keyPress));
	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<Editbox, EventMouseOver>(this, &Editbox::mouseOver));
	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<Editbox, EventMouseOut>(this, &Editbox::mouseOut));
	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<Editbox, EventTick>(this, &Editbox::newTick));
	m_cursor = VideoSystem::getInstance()->createSurface("images/misc/textcursor.png");
}

Editbox::~Editbox()
{
}

void Editbox::draw(const VideoSystem* const screen)
{
	Rect r(-m_textOffsetX, 0, m_width - 6, m_height);
	screen->drawFilledRect(m_absX, m_absY, m_width, m_height, m_hovered ? Color::GRAY80 : Color::GRAY70);
	screen->drawRect(m_absX, m_absY, m_width, m_height, Color::BLACK);
	screen->drawText(m_text, m_absX+3, m_absY+2, Color::BLACK, Font::SIZE_SMALL, &r);
	if(hasFocus() && m_timer < 0.5f)
	{
		screen->drawRect(m_absX + m_cursorPos + m_textOffsetX, m_absY + 2, 0, m_height - 4, Color::BLACK);
	}

	Widget::draw(screen);
}

std::string Editbox::getText() const
{
	return m_text;
}

void Editbox::setText(const std::string& text)
{
	m_text = text;
}

void Editbox::mouseOver(EventMouseOver* event)
{
	m_hovered = true;
}

void Editbox::mouseOut(EventMouseOut* event)
{
	m_hovered = false;
}

void Editbox::newTick(EventTick* event)
{
	m_timer += event->m_delayTime;
	if(m_timer >= 1.0f)
	{
		m_timer -= 1.0f;
	}
}

void Editbox::keyPress(EventKey* event)
{
	if(event->m_isKeyDown && hasFocus())
	{
		bool newText = false, newCursor = false;
		if(event->m_charCode == 8) //backspace
		{
			if(m_cursorPosIndex > 0)
			{
				m_text.erase(m_text.begin() + m_cursorPosIndex - 1);
				m_cursorPosIndex--;
				newText = newCursor = true;
			}
		}
		else if(event->m_charCode == 127) //del
		{
			if(m_cursorPosIndex < m_text.size())
			{
				m_text.erase(m_text.begin() + m_cursorPosIndex);
				newText = newCursor = true;
			}
		}
		else if(event->m_keyCode == 20) //left
		{
			if(m_cursorPosIndex > 0)
			{
				--m_cursorPosIndex;
				newCursor = true;
			}
		}
		else if(event->m_keyCode == 19) //right
		{
			if(m_cursorPosIndex < m_text.size())
			{
				++m_cursorPosIndex;
				newCursor = true;
			}
		}
		else if(event->m_keyCode == 22) //home
		{
			newCursor = (m_cursorPosIndex != 0);
		  m_cursorPosIndex = 0;
		}
		else if(event->m_keyCode == 23) //end
		{
			newCursor = (m_cursorPosIndex != (unsigned int)m_text.size());
		  m_cursorPosIndex = (unsigned int)m_text.size();
		}
		else if(event->m_charCode >= 32 && event->m_charCode < 127)
		{
			m_text.insert(m_text.begin() + m_cursorPosIndex, event->m_charCode);
			++m_cursorPosIndex;
			newText = newCursor = true;
		}

		if(newCursor || newText)
		{
			m_cursorPos = VideoSystem::getInstance()->getDefaultFont()->getTextWidth(m_text.substr(0, m_cursorPosIndex), Font::SIZE_SMALL) + 2;
			
			if(VideoSystem::getInstance()->getDefaultFont()->getTextWidth(m_text, Font::SIZE_SMALL) + m_textOffsetX <= m_width - 3 && m_textOffsetX < 0)
			{
				m_textOffsetX = (m_cursorPos + m_textOffsetX) - VideoSystem::getInstance()->getDefaultFont()->getTextWidth(m_text, Font::SIZE_SMALL);
			}
	
			if(static_cast<int>(m_cursorPos) + m_textOffsetX < 2)
			{
				m_textOffsetX = 2 - m_cursorPos;
			}
			else if(static_cast<int>(m_cursorPos) + m_textOffsetX > m_width - 3)
			{
				m_textOffsetX = m_width - 3 - m_cursorPos;
			}
			if(VideoSystem::getInstance()->getDefaultFont()->getTextWidth(m_text, Font::SIZE_SMALL) < m_width)
			{
				m_textOffsetX = 0;
			}

			if(newText)
			{
				doCommand();
			}
		}
	}
}
