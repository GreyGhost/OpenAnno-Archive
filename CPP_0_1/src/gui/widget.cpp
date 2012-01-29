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
#include "widgetmanager.h"
#include "widget.h"

Widget::Widget(int x, int y) : m_relX(x), m_relY(y), m_absX(x), m_absY(y), m_width(0), m_height(0), m_parent(NULL), m_isEnabled(true), m_isVisible(true), m_level(0), m_cursor(NULL)
{
	WidgetManager::getInstance()->registerWidget(this);
}


Widget::~Widget()
{
	Widgets::iterator it;
	while((it = m_children.begin()) != m_children.end())
	{
		delete (*it);
	}

	if(m_parent)
	{
		m_parent->removeChild(this);
	}

	WidgetManager::getInstance()->unregisterWidget(this);
}

void Widget::addChild(Widget* child)
{
	child->setParent(this);
	m_children.push_back(child);
}

void Widget::removeChild(Widget* child)
{
	m_children.remove(child);
}

void Widget::setParent(Widget* parent)
{
	if(m_parent)
	{
		m_parent->removeChild(this);
	}
	m_parent = parent;
	recalculateAbsPos();
	recalculateLevel();
}

SurfacePtr Widget::getCursor() const
{
	return m_cursor;
}

void Widget::recalculateAbsPos()
{
	if(m_parent)
	{
		m_absX = m_relX + m_parent->getAbsX();
		m_absY = m_relY + m_parent->getAbsY();
	}
	else
	{
		m_absX = m_relX;
		m_absY = m_relY;
	}

	for(Widgets::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		(*it)->recalculateAbsPos();
	}
}

void Widget::setRelPos(int x, int y)
{
	m_relX = x;
	m_relY = y;
	recalculateAbsPos();
}

void Widget::enable()
{
	m_isEnabled = true;
}

void Widget::disable()
{
	m_isEnabled = false;
}

void Widget::captureMouse()
{
	WidgetManager::getInstance()->captureMouse(this);
}

void Widget::releaseMouseCapture()
{
	WidgetManager::getInstance()->releaseMouseCapture(this);
}

bool Widget::hasMouseCaptured()
{
	return WidgetManager::getInstance()->hasMouseCaptured(this);
}

void Widget::setFocus()
{
	WidgetManager::getInstance()->setFocus(this);
}

bool Widget::hasFocus()
{
	return WidgetManager::getInstance()->hasFocus(this);
}

void Widget::draw(const VideoSystem* const screen)
{
	for(Widgets::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		if((*it)->isVisible())
		{
			(*it)->draw(screen);
		}
	}
	if(hasFocus())
		VideoSystem::getInstance()->drawRect(m_absX, m_absY, m_width, m_height, Color(0.8f, 0.0f, 0.0f));
}

void Widget::recalculateLevel()
{
	if(m_parent)
	{
		m_level = m_parent->getLevel() + 1;
	}
	else
	{
		m_level = 0;
	}

	for(Widgets::iterator it = m_children.begin(); it != m_children.end(); it++)
	{
		(*it)->recalculateLevel();
	}
}

unsigned int Widget::getLevel()
{
	return m_level;
}

bool Widget::isPointInWidget(int x, int y)
{
	return ((static_cast<unsigned>(x - m_absX) < static_cast<unsigned>(m_width))  && (static_cast<unsigned>(y - m_absY) < static_cast<unsigned>(m_height)));
}

bool Widget::isEnabled()
{
	if(m_parent)
	{
		return m_isEnabled && m_parent->isEnabled();
	}

	return m_isEnabled;
}

void Widget::sendEvent(Event* event)
{
	WidgetManager::getInstance()->sendEvent(event, this);
}

std::string Widget::getTooltipText(void) const
{
	return m_strTooltip;
}

void Widget::setTooltipText(std::string tooltip)
{
	m_strTooltip = tooltip;
}

void Widget::show()
{
	m_isVisible = true;
	WidgetManager::getInstance()->updateWidgetUnderMouse();
}

void Widget::hide()
{
	m_isVisible = false;
	WidgetManager::getInstance()->updateWidgetUnderMouse();
}

bool Widget::isVisible()
{
	if(m_parent)
	{
		return m_parent->isVisible() && m_isVisible;
	}
	return m_isVisible;
}
