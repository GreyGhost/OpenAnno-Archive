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

#include "eventmouseclick.h"
#include "../widget.h"

const EventType EventMouseClick::TYPE = Event::getNewTypeId();

EventMouseClick::EventMouseClick(Button button, int absX, int absY, bool isButtonDown) : Event(EventMouseClick::TYPE), m_absX(absX), m_absY(absY), m_button(button), m_isButtonDown(isButtonDown)
{
}

EventMouseClick::~EventMouseClick()
{
}

int EventMouseClick::getRelX(Widget* relativeTo) const
{
	return m_absX - relativeTo->getAbsX();
}

int EventMouseClick::getRelY(Widget* relativeTo) const
{
	return m_absY - relativeTo->getAbsY();
}

bool EventMouseClick::shouldBeSendTo(Widget* target)
{
	return (target->isPointInWidget(m_absX, m_absY) && target->isEnabled()) || target->hasMouseCaptured();
}
