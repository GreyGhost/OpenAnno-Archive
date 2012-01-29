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

#ifndef SELECTBOX_H
#define SELECTBOX_H

#include <string>
#include <vector>
#include "video/surface.h"
#include "video/videosystem.h"
#include "gui/selector.h"
#include "gui/events/eventmouseclick.h"
#include "gui/scrollbar.h"
#include "gui/events/eventcommand.h"

class SelectBox : public Selector
{
public:
	SelectBox(int x, int y, int width, int height, bool hasScollBar = false, int commandId = -1);
	~SelectBox();
	void draw(const VideoSystem* const screen);
	void mouseClick(EventMouseClick* event);
	void goDown(int count);	// see goUp - here you go down ;)
	void goUp(int count);	// go counts page up, <1 you go to the uppest page
	void onCommand(EventCommand* event);
	void addItem(const std::string& caption, const std::string& name = ""); // caption will you see, name only for short identify
	int getPage();	// get actual page
	int getMaxPages();	// get value of maximal pages
	std::string& getSelected();
private:
	int m_textOffsetX;
	int m_textOffsetY;
	int m_maxPages;
	int m_page;		// actual page
	int m_maxItemsPerPage;
	bool m_hasScrollBar;
	int m_internWidth, m_internHeight;
	ScrollBar* m_scrollBarHor;
	ScrollBar* m_scrollBarVer;
};

#endif
