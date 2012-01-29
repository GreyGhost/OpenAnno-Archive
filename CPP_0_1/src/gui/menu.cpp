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

#include "menu.h"
#include <assert.h>
#include <iostream>
#include "widgetmanager.h"
#include "events/eventmousemove.h"
#include "events/eventmouseclick.h"
#include "events/eventcommand.h"


MenuItem::MenuItem(int x, int y, const std::string& title, const std::string& icon, const std::string& iconHighlighted, int id) : Widget(x, y), m_icon(NULL), m_iconHighlighted(NULL), m_title(title), m_id(id)
{
	m_icon = VideoSystem::getInstance()->createSurface(icon);
	m_iconHighlighted = VideoSystem::getInstance()->createSurface(iconHighlighted);
	if(m_icon.isNotNull())
	{
		m_width = m_icon->getWidth() + VideoSystem::getInstance()->getDefaultFont()->getTextWidth(m_title, Font::SIZE_SMALL) + 5;
		m_height = m_icon->getHeight();
		m_textOffsetX = m_icon->getWidth() + 5;
		m_textOffsetY = (m_icon->getHeight() - VideoSystem::getInstance()->getDefaultFont()->getTextHeight(m_title, Font::SIZE_SMALL))/2;
		if(m_iconHighlighted.isNull())
		{
			m_iconHighlighted = m_icon;
		}
	}
	else
	{
		m_width = VideoSystem::getInstance()->getDefaultFont()->getTextWidth(m_title, Font::SIZE_SMALL) ;
		m_height = VideoSystem::getInstance()->getDefaultFont()->getTextHeight(m_title, Font::SIZE_SMALL);
		m_textOffsetX = 0;
		m_textOffsetY = 0;
	}
}

MenuItem::~MenuItem()
{
	m_title.clear();
}

void MenuItem::draw(const VideoSystem* const screen)
{
	if(m_isEnabled)
	{
		if(m_iconHighlighted.isNotNull())
		{
			screen->drawSurface(m_iconHighlighted, m_absX, m_absY);
		}
		screen->drawText(m_title, m_absX+m_textOffsetX, m_absY+m_textOffsetY, Color::BLACK, Font::SIZE_SMALL);
	}
	else
	{
		if(m_icon.isNotNull())
		{
			screen->drawSurface(m_icon, m_absX, m_absY);
		}
		screen->drawText(m_title, m_absX+m_textOffsetX, m_absY+m_textOffsetY, Color::GRAY30, Font::SIZE_SMALL);
	}

	Widget::draw(screen);
}

int MenuItem::getId() const
{
	return m_id;
}


Menu::Menu(int x, int y, const std::string& background) : Dialog(x, y, -1, -1, background), m_itemSpaceing(10)
{
	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<Menu, EventMouseMove>(this, &Menu::mouseMove));
	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<Menu, EventMouseClick>(this, &Menu::mouseClick));
}


Menu::~Menu()
{
}

void Menu::mouseClick(EventMouseClick* event)
{
	if(event->m_button == EventMouseClick::BTN_LEFT && !event->m_isButtonDown)
	{
		for(Widgets::iterator it = m_children.begin(); it != m_children.end(); ++it)
		{
			if((*it)->isPointInWidget(event->m_absX, event->m_absY))
			{
				if((*it)->hasFocus())
				{
					WidgetManager::getInstance()->sendEvent(new EventCommand(static_cast<MenuItem*>(*it)->getId()), this);
				}
			}
		}
	}
}

void Menu::mouseMove(EventMouseMove* event)
{
	for(Widgets::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		if((*it)->isPointInWidget(event->m_absX, event->m_absY))
		{
			(*it)->enable();
		}
		else
		{
			(*it)->disable();
		}
	}
}

void Menu::addItem(const std::string& title, const std::string& icon, const std::string& iconHighlighted, int commandId)
{
	int posY = 50;

	if(m_children.size())
	{
		Widget* widget = (*m_children.rbegin());
		posY = widget->getRelY() + widget->getHeight() + m_itemSpaceing;
	}
	MenuItem* menuItem = new MenuItem(50, posY, title, icon, iconHighlighted, commandId);
	menuItem->disable();
	if(menuItem->getWidth() > m_width)
	{
		m_width = menuItem->getWidth();
	}
	addChild(menuItem);
}
