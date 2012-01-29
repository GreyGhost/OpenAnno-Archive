 /***************************************************************************
 *   Copyright (C) 2006 by Sven Rech                                       *
 *   svenrech@gmx.de                                                       *
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

#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

#include <string>
#include <vector>
#include "gui/widget.h"
#include "gui/scrollbar.h"
#include "gui/events/eventcommand.h"
#include "video/videosystem.h"

class TextViewer : public Widget
{
public:
	TextViewer(int x, int y, int width, int height, std::string text="", bool hasScrollBar=true);
	void setText(std::string text);
	void draw(const VideoSystem* const screen);
	void onCommand(EventCommand* event);
private:
	int m_internWidth;
	bool m_hasScrollBar;
	int m_page;
	int m_maxItemsPerPage;
	int m_maxPages;
	std::vector<std::string> m_textList;
	ScrollBar* m_scrollbar;
};

#endif
