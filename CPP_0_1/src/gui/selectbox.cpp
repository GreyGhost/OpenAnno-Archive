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

#include "selectbox.h"
#include "scrollbar.h"
#include "gui/widgetmanager.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>

SelectBox::SelectBox(int x, int y, int width, int height, bool hasScrollBar ,int commandId) : Selector(x, y, Selector::VERTICAL_SELECTOR, commandId), m_hasScrollBar(hasScrollBar)
{
	m_width = width;
	m_height = height;
	if(m_hasScrollBar == true)
	{
		// Resize rect for the scrollbars
		m_scrollBarVer = new ScrollBar(x, y, height, 1, ScrollBar::VERTICAL, 0);
		addChild(m_scrollBarVer);
		m_scrollBarVer->setRelPos(width - m_scrollBarVer->getWidth(), 0);

		m_scrollBarHor = new ScrollBar(x, y, m_width - m_scrollBarVer->getWidth(), 1, ScrollBar::HORIZONTAL, 0);
		addChild(m_scrollBarHor);
		m_scrollBarHor->setRelPos(0, m_height - m_scrollBarHor->getHeight());

		m_scrollBarVer->setSize(m_height - m_scrollBarHor->getHeight());		// Design

		m_internWidth = m_width - m_scrollBarVer->getWidth();
		m_internHeight = m_height - m_scrollBarHor->getHeight();

		// EventListener would only use from ScrollBar(s)
		WidgetManager::getInstance()->addEventListener(new SpecificEventListener<SelectBox, EventCommand>(this, &SelectBox::onCommand));
	}
	else
	{
		m_internWidth = m_width;
		m_internHeight = m_height;
	}
	m_maxItemsPerPage = m_internHeight / VideoSystem::getInstance()->getDefaultFont()->getTextHeight(" ", Font::SIZE_SMALL);
	m_maxPages = ((int) m_itemsTitle.size() - 1) / m_maxItemsPerPage + 1;
	m_page = 1;
}

SelectBox::~SelectBox()
{
}

void SelectBox::draw(const VideoSystem* const screen)
{
	screen->drawRect(m_absX, m_absY, m_width, m_height, Color::BLACK);
	int x = m_absX + 1;
	int y = m_absY + 1;

	int textHeight = screen->getDefaultFont()->getTextHeight(" ", Font::SIZE_SMALL);
	for(int i = (m_page-1)*m_maxItemsPerPage; i < static_cast<int>(m_itemsTitle.size()) && i < m_page*m_maxItemsPerPage; ++i)
	{
		if(i == m_selectedItem)
		{
			screen->drawFilledRect(x, y, m_internWidth - 1, textHeight, Color(0.3f, 0.7f, 0.9f));
			screen->drawText(m_itemsTitle[i], x, y, Color::BLACK);
		}
		else
		{
			screen->drawText(m_itemsTitle[i], x, y, Color::GRAY40);
		}
		y += textHeight;
	}
	Widget::draw(screen);
}

void SelectBox::mouseClick(EventMouseClick* event)
{
	if (event->m_button == EventMouseClick::BTN_LEFT)
	{
		if(hasFocus() && event->m_isButtonDown && (event->getRelX(this) < m_internWidth) && (event->getRelY(this) < m_internHeight))
		{
			m_selectedItem = (event->m_absY - m_absY) / VideoSystem::getInstance()->getDefaultFont()->getTextHeight(" ", Font::SIZE_SMALL);
			if ((m_selectedItem < 0) || (m_selectedItem >= static_cast<signed int>(m_itemsTitle.size())))
			{
				m_selectedItem = -1;
			}
			else
			{
				m_selectedItem += m_maxItemsPerPage*(m_page-1);
			}
			doCommand();
		}
	}
	else if (event->m_button == EventMouseClick::BTN_WHEELUP)
		m_scrollBarVer->setCount(m_scrollBarVer->getCount()-1);
	else if (event->m_button == EventMouseClick::BTN_WHEELDOWN)
		m_scrollBarVer->setCount(m_scrollBarVer->getCount()+1);
}

void SelectBox::onCommand(EventCommand* event)
{
	if(m_hasScrollBar == true)
	{
		if(event->getSender() == m_scrollBarVer)
		{
			m_page = m_scrollBarVer->getCount();
		}
	}
}

void SelectBox::goDown(int count)
{
	m_page += count;
	if(count < 0 || m_page > m_maxPages)
		m_page = m_maxPages;
}

void SelectBox::goUp(int count)
{
	m_page -= count;
	if(m_page < 1)
		m_page = 1;
}
int SelectBox::getPage()
{
	return m_page;
}

int SelectBox::getMaxPages()
{
	return m_maxPages;
}

void SelectBox::addItem(const std::string& caption, const std::string& name )
{
	if(m_selectedItem == -1)
	{
		m_selectedItem = 0;
	}
	m_itemsTitle.push_back(caption);
	if(name == "")
	{
		m_itemsName.push_back(caption);
	}
	else
	{
		m_itemsName.push_back(name);
	}
	m_maxPages = ((int) m_itemsTitle.size() - 1) / m_maxItemsPerPage + 1;
	if(m_hasScrollBar == true)
		m_scrollBarVer->setMaxCounts(m_maxPages);
}

std::string& SelectBox::getSelected()
{
	return m_itemsName[m_selectedItem];
}
