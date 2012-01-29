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

#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <string>
#include <vector>
#include "video/surface.h"
#include "video/videosystem.h"
#include "gui/commandwidget.h"
#include "gui/events/eventmouseclick.h"
#include "gui/events/eventmousemove.h"

class ScrollBar : public CommandWidget
{
public:
	enum Type
	{
		VERTICAL,
		HORIZONTAL
	};

	ScrollBar(int x, int y, int size, int counts, Type type = VERTICAL, int commandId = -1);
	virtual ~ScrollBar();
	void draw(const VideoSystem* const screen);
	void onMouseClick(EventMouseClick* event);
	void onMouseMove(EventMouseMove* event);
	int getCount();
	void setCount(int count);
	void setMaxCounts(int counts);
	void setSize(int size);

protected:
	void calculateBar();
	int m_maxcounts;
	Type m_type;
	int m_count;
	int m_barLength;
	int m_barPos;
	bool m_drag;
	int m_dragPos;
	SurfacePtr m_arrows[2];
};


#endif
