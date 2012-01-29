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

#ifndef EVENTMOUSECLICK_H
#define EVENTMOUSECLICK_H

#include "event.h"

class EventMouseClick : public Event
{
public:
	enum Button
	{
		BTN_LEFT,
		BTN_MIDDLE,
		BTN_RIGHT,
		BTN_WHEELUP,
		BTN_WHEELDOWN
	};
	EventMouseClick(Button button, int absX, int absY, bool isButtonDown);
	virtual ~EventMouseClick();
	int getRelX(Widget* relativeTo) const;
	int getRelY(Widget* relativeTo) const;
	bool shouldBeSendTo(Widget* target);

	const int m_absX, m_absY;
	const unsigned int m_button;
	const bool m_isButtonDown;

	static const EventType TYPE;
};


#endif
