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

#ifndef EVENT_H
#define EVENT_H

/** \defgroup eventsandwidgets Events and widgets
 *  Group for events and widgets
 *  \{
 */

/** \page explanation Explanation
 *  The \ref WidgetManager handles the events coming from the SDL subsystem.<br />
 *  All events must derive from Event. A widget can register itself for the event by calling \ref WidgetManager::addEventListener or (if you want to unregister the event handler) \ref WidgetManager::remEventListener.<br />
 *  When an event arrives, the \ref Event object \link Event::shouldBeSendTo decides\endlink to which of the widgets which have registered themselves for the Event it is sent.
 *  */

typedef unsigned int EventType;
class Widget;

class Event
{
public:
	Event(int type);
	virtual ~Event();
	EventType getType() const { return m_type; }
	void setSender(Widget* sender);
	virtual Widget* getSender() const { return m_sender; };
	virtual bool shouldBeSendTo(Widget* target);
	
	static EventType getNewTypeId();
protected:
	const EventType m_type;
	Widget* m_sender;
private:
	static EventType m_eventTypeIdCounter;
};
/** \} */

#endif
