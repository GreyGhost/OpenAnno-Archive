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

#include "scrollbar.h"
#include "widgetmanager.h"


ScrollBar::ScrollBar(int x, int y, int size, int counts, Type type, int commandId) : CommandWidget(x, y, commandId), m_maxcounts(counts), m_type(type)
{
	m_drag = false;
	m_count = 1;
	m_maxcounts = 1;
	if(m_type == VERTICAL)
	{
		m_arrows[0] = VideoSystem::getInstance()->createSurface("images/hud/up.png");
		m_arrows[1] = VideoSystem::getInstance()->createSurface("images/hud/down.png");
		if (m_arrows[0] != NULL)
			m_width = m_arrows[0]->getWidth();
		m_height = size;
	}
	else
	{
		m_arrows[0] = VideoSystem::getInstance()->createSurface("images/hud/left.png");
		m_arrows[1] = VideoSystem::getInstance()->createSurface("images/hud/right.png");
		m_width = size;
		if (m_arrows[0] != NULL)
			m_height = m_arrows[0]->getHeight();
	}
	calculateBar();

	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<ScrollBar, EventMouseClick>(this, &ScrollBar::onMouseClick));
	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<ScrollBar, EventMouseMove>(this, &ScrollBar::onMouseMove));
}

ScrollBar::~ScrollBar()
{
}

void ScrollBar::onMouseClick(EventMouseClick* event)
{
	if (event->m_button == EventMouseClick::BTN_LEFT)
	{
		if (!event->m_isButtonDown)
		{
			m_drag = false;
			if(hasMouseCaptured())
			{
				releaseMouseCapture();
			}
		}
		else
		{
			// First test for left or up button
			if (event->getRelX(this) >= 0 && event->getRelX(this) <= m_arrows[0]->getWidth() &&
			   event->getRelY(this) >= 0 && event->getRelY(this) <= m_arrows[0]->getHeight() )
			{
				m_count--;
				if(m_count < 1)
					m_count = 1;
				doCommand();
			} // test for down or right button
			else if (event->getRelX(this) >= m_width - m_arrows[1]->getWidth() && event->getRelX(this) <= m_width &&
					 event->getRelY(this) >= m_height - m_arrows[1]->getHeight() && event->getRelY(this) <= m_height)
			{
				m_count++;
				if(m_count > m_maxcounts)
					m_count = m_maxcounts;
				doCommand();
			}
			else if ((m_type == VERTICAL) && (event->getRelY(this) >= m_barPos) &&
						(event->getRelY(this) <= m_barPos+m_barLength))
			{
				m_drag = true;
				m_dragPos = event->getRelY(this);
				captureMouse();
			}
			else if ((m_type == HORIZONTAL) && (event->getRelX(this) >= m_barPos) &&
						(event->getRelX(this) <= m_barPos+m_barLength))
			{
				m_drag = true;
				m_dragPos = event->getRelX(this);
				captureMouse();
			}
			else
			{
				if (m_type == VERTICAL)
					m_count = ((event->getRelY(this)-m_arrows[0]->getHeight()) / m_barLength) + 1;
				else
					m_count = ((event->getRelX(this)-m_arrows[0]->getWidth()) / m_barLength) + 1;
				doCommand();
			}
		}
	}
	else if (event->m_button == EventMouseClick::BTN_WHEELUP)
	{
		m_count--;
		if(m_count < 1)
			m_count = 1;
		doCommand();
	} 
	else if (event->m_button == EventMouseClick::BTN_WHEELDOWN)
	{
		m_count++;
		if(m_count > m_maxcounts)
			m_count = m_maxcounts;
		doCommand();
	}
	calculateBar();
}

void ScrollBar::calculateBar()
{
	if(m_type == VERTICAL)
	{
		if ((m_arrows[1] != NULL) && (m_arrows[0] != NULL))
		{
			m_barLength = (m_height - m_arrows[0]->getHeight() - m_arrows[1]->getHeight()) / m_maxcounts;
			m_barPos = m_arrows[0]->getHeight() + m_barLength*(m_count-1);
		}
	}
	else
	{
		if ((m_arrows[1] != NULL) && (m_arrows[0] != NULL))
		{
			m_barLength = (m_width - m_arrows[0]->getWidth() - m_arrows[1]->getWidth()) / m_maxcounts;
			m_barPos = m_arrows[0]->getWidth() + m_barLength*(m_count-1);
		}
	}
}

void ScrollBar::onMouseMove(EventMouseMove* event)
{
	if (m_drag)
	{
		int diff;
		int arrowSize;
		if (m_type == VERTICAL)
		{
			diff = event->getRelY(this) - m_dragPos;
			arrowSize = m_arrows[0]->getHeight();
		}
		else
		{
			diff = event->getRelX(this) - m_dragPos;
			arrowSize = m_arrows[0]->getWidth();
		}
		m_count = ((m_barPos-arrowSize+diff) / m_barLength) + 1;
		if (m_count > m_maxcounts)
			m_count = m_maxcounts;
		calculateBar();
		m_dragPos = m_barPos;
		doCommand();
	}
}

void ScrollBar::setSize(int size)
{
	if(m_type == VERTICAL)
		m_height = size;
	else
		m_width = size;
	calculateBar();
}


int ScrollBar::getCount()
{
	return m_count;
}

void ScrollBar::setCount(int count)
{
	m_count = count;
	if (m_count < 1)
		m_count = 1;
	if (m_count > m_maxcounts)
		m_count = m_maxcounts;
	calculateBar();
	doCommand();
}

void ScrollBar::setMaxCounts(int counts)
{
	m_maxcounts = counts;
	calculateBar();
}

void ScrollBar::draw(const VideoSystem* const screen)
{
	screen->drawRect(m_absX, m_absY, m_width, m_height, Color(1.0f, 0.5f, 0.0f));
	const int border = 3;
	if(m_type == VERTICAL)
	{
		screen->drawSurface(m_arrows[0], m_absX, m_absY);
		screen->drawSurface(m_arrows[1], m_absX, m_absY + m_height - m_arrows[1]->getHeight());
		screen->drawFilledRect(m_absX, m_absY+m_barPos, m_width, m_barLength, Color(0.7f, 0.5f, 0.2f));
		screen->drawFilledRect(m_absX+border, m_absY+m_barPos+border, m_width-(2*border), m_barLength-(2*border), Color::BLACK);
	}
	else
	{
		screen->drawSurface(m_arrows[0], m_absX, m_absY);
		screen->drawSurface(m_arrows[1], m_absX + m_width - m_arrows[1]->getWidth(), m_absY);
		screen->drawFilledRect(m_absX+m_barPos, m_absY, m_barLength, m_height, Color(0.7f, 0.5f, 0.2f));
		screen->drawFilledRect(m_absX+m_barPos+border, m_absY+border, m_barLength-(2*border), m_height-(2*border), Color::BLACK);
	}

	Widget::draw(screen);
}
