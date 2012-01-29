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

#include "checkbox.h"
#include "widgetmanager.h"
#include "events/eventcommand.h"

Checkbox::Checkbox(int x, int y, const std::string& caption, int commandId) : CommandWidget(x, y, commandId), m_caption(caption), m_checked(false)
{
	m_checkedBox = VideoSystem::getInstance()->createSurface("images/hud/checkbox_checked.png");
	m_box = VideoSystem::getInstance()->createSurface("images/hud/checkbox.png");
	if(m_box.isNotNull())
	{
		m_width = m_box->getWidth() + VideoSystem::getInstance()->getDefaultFont()->getTextWidth(m_caption, Font::SIZE_SMALL) + 5;
		m_height = m_box->getHeight();
	}
	else
	{
		m_width = 16;
		m_height = 16;
	}

	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<Checkbox, EventMouseClick>(this, &Checkbox::mouseClick));
}

Checkbox::~Checkbox()
{
}

void Checkbox::draw(const VideoSystem* const screen)
{
	if(m_checked)
	{
		if(m_checkedBox.isNotNull())
		{
			screen->drawSurface(m_checkedBox, m_absX, m_absY);
		}
	}
	else
	{
		if(m_box.isNotNull())
		{
			screen->drawSurface(m_box, m_absX, m_absY);
		}
	}
	if(m_box.isNotNull())
	{
		screen->drawText(m_caption, m_absX + 5 + m_box->getWidth(), m_absY, Color::BLACK, Font::SIZE_SMALL);
	}
	else if(m_checkedBox.isNotNull())
	{
		screen->drawText(m_caption, m_absX + 5 + m_checkedBox->getWidth(), m_absY, Color::BLACK, Font::SIZE_SMALL);
	}
	else
	{
		screen->drawText(m_caption, m_absX, m_absY, Color::BLACK, Font::SIZE_SMALL);
	}
	Widget::draw(screen);
}

void Checkbox::mouseClick(EventMouseClick* event)
{
	if(event->m_button == EventMouseClick::BTN_LEFT && isPointInWidget(event->m_absX, event->m_absY))
	{
		if(!event->m_isButtonDown && hasFocus())
		{
			m_checked = !m_checked;
			doCommand();
		}
	}
}
