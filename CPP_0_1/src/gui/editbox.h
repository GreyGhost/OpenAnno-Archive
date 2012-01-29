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

#ifndef EDITBOX_H
#define EDITBOX_H

#include <string>
#include "video/videosystem.h"
#include "gui/commandwidget.h"
#include "gui/events/eventkey.h"
#include "gui/events/eventmouseout.h"
#include "gui/events/eventmouseover.h"
#include "gui/events/eventtick.h"

class Editbox : public CommandWidget
{
public:
	Editbox(int x, int y, int width, int commandId = -1);
	virtual ~Editbox();
	void draw(const VideoSystem* const screen);
	std::string getText() const;
	void setText(const std::string& text);
	void keyPress(EventKey* event);
	void mouseOut(EventMouseOut* event);
	void mouseOver(EventMouseOver* event);
	void newTick(EventTick* event);

protected:
	std::string m_text;
	unsigned int m_cursorPos;
	unsigned int m_cursorPosIndex;
	int m_textOffsetX;
	bool m_hovered;
	float m_timer;
};


#endif
