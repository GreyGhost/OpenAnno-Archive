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

#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <vector>
#include <list>
#include "video/surface.h"
#include "video/videosystem.h"
#include "gui/selector.h"
#include "gui/events/eventmouseclick.h"
#include "gui/scrollbar.h"
#include "gui/events/eventcommand.h"

class Table: public CommandWidget
{
public:
	typedef std::vector<std::string> row;
	typedef std::vector<row> rows;
	
	Table(int x, int y, int width, int height, row& titles, bool hasScollBar = false, int commandId = -1);
	~Table();
	void draw(const VideoSystem* const screen);
	void mouseClick(EventMouseClick* event);
	void mouseMove(EventMouseClick* event);
	void goDown(int count);	// see goUp - here you go down ;)
	void goUp(int count);	// go counts page up, <1 you go to the uppest page
	void onCommand(EventCommand* event);
	void sortByColumn(int column);
	void setColumnSize(int column, int size);

	void addRow(const row& data);
	row& getRowByItem(std::string& item, int column); //searchs for a row with the string "item" in column "column"
	void setFieldInRow(row& the_row, int column, std::string& entry) { the_row[column] = entry; }
	std::string& getFieldInRow(row& the_row, int column) {return the_row[column];}
	row& getSelectedRow();
	
	int getPage();	// get actual page
	int getMaxPages();	// get value of maximal pages
private:
	int m_textOffsetX;
	int m_textOffsetY;
	int m_maxPages;
	int m_page;		// actual page
	int m_maxItemsPerPage;
	bool m_hasScrollBar;
	int m_internWidth, m_internHeight;
	int m_selectedItem;
	ScrollBar* m_scrollBarHor;
	ScrollBar* m_scrollBarVer;
	rows m_table;
	int m_columns;
	row m_titles;
	std::vector<int> m_columnWidths;
};

#endif
