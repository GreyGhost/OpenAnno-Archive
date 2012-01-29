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

#include "eventkey.h"
#ifdef _WIN32
# include <SDL.h>
#else
# include <SDL/SDL.h>
#endif

const EventType EventKey::TYPE = Event::getNewTypeId();

EventKey::EventKey(unsigned char keyCode, bool isKeyDown, int modifiers, unsigned char charCode) : Event(EventKey::TYPE), m_keyCode(keyCode), m_charCode(charCode), m_isKeyDown(isKeyDown), m_modifiers(modifiers)
{
}

EventKey::~EventKey()
{
}
