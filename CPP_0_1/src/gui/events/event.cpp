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

#include "event.h"
#include "gui/widget.h"

EventType Event::m_eventTypeIdCounter = 0;

Event::Event(int type) : m_type(type), m_sender(NULL)
{
}

Event::~Event()
{
}

EventType Event::getNewTypeId()
{
	return ++m_eventTypeIdCounter;
}

void Event::setSender(Widget* sender)
{
	m_sender = sender;
}

bool Event::shouldBeSendTo(Widget* target)
{
	return target->isEnabled();
}
