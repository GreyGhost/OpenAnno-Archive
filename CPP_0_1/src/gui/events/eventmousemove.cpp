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

#include "eventmousemove.h"
#include "../widgetmanager.h"
#include "../widget.h"

const EventType EventMouseMove::TYPE = Event::getNewTypeId();

EventMouseMove::EventMouseMove(int absX, int absY, int relX, int relY) : Event(EventMouseMove::TYPE), m_absX(absX), m_absY(absY), m_relX(relX), m_relY(relY)
{
}

EventMouseMove::~EventMouseMove()
{
}

int EventMouseMove::getRelX(Widget* relativeTo) const
{
	return m_absX - relativeTo->getAbsX();
}

int EventMouseMove::getRelY(Widget* relativeTo) const
{
	return m_absY - relativeTo->getAbsY();
}

bool EventMouseMove::shouldBeSendTo(Widget* target)
{
	if(WidgetManager::getInstance()->hasMouseCaptured(target))
	{
		return true;
	}

	return (WidgetManager::getInstance()->hasMouseCaptured(NULL) || target->getParent() == NULL) && target->isPointInWidget(m_absX, m_absY) && target->isEnabled();
}
