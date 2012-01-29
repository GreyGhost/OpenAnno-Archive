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

#include "table.h"
#include "scrollbar.h"
#include "gui/widgetmanager.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <exception>
#include <algorithm>

Table::Table(int x, int y, int width, int height, row& titles, bool hasScrollBar, int commandId) : CommandWidget(x, y, commandId), m_hasScrollBar(hasScrollBar)
{
	m_width = width;
	m_height = height;
	m_columns = (int) titles.size();
	m_titles = titles;
	m_columnWidths.assign(m_columns, width/m_columns);
	m_selectedItem = -1;
	if(m_hasScrollBar == true)
	{
		// Resize rect for the scrollbars
		m_scrollBarVer = new ScrollBar(x, y, height, 1, ScrollBar::VERTICAL, 0);
		addChild(m_scrollBarVer);
		m_scrollBarVer->setRelPos(width - m_scrollBarVer->getWidth(), 0);

		m_internWidth = m_width - m_scrollBarVer->getWidth();
		m_internHeight = m_height;

		// EventListener would only use from ScrollBar(s)
		WidgetManager::getInstance()->addEventListener(new SpecificEventListener<Table, EventCommand>(this, &Table::onCommand));
	}
	else
	{
		m_internWidth = m_width;
		m_internHeight = m_height;
	}
	m_maxItemsPerPage = (m_internHeight / VideoSystem::getInstance()->getDefaultFont()->getTextHeight(" ", Font::SIZE_SMALL))-1;
	m_maxPages = ((int) m_table.size() - 1) / m_maxItemsPerPage + 1;
	m_page = 1;

	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<Table, EventMouseClick>(this, &Table::mouseClick));
}

Table::~Table()
{
}

void Table::draw(const VideoSystem* const screen)
{
	screen->drawRect(m_absX, m_absY, m_width, m_height, Color::BLACK);
	int x = m_absX;
	int y = m_absY;

	int textHeight = screen->getDefaultFont()->getTextHeight(" ", Font::SIZE_SMALL);

	//render titles:
	int x2 = x;
	std::vector<int>::iterator widthIt = m_columnWidths.begin();
	for (row::iterator it = m_titles.begin(); it != m_titles.end(); it++, widthIt++)
	{
		screen->drawFilledRect(x2, y, *widthIt, textHeight, Color(0.8f, 0.4f, 0.4f));
		screen->drawText(*it, x2, y, Color::BLACK);
		x2 += *widthIt;
	}
	y += textHeight;

	//search for first row to render:
	int first = (m_page-1)*m_maxItemsPerPage;
	rows::iterator firstIt = m_table.begin()+first;

	//render rows:
	for (int elem = first; (elem < first+m_maxItemsPerPage) && (elem < (int)m_table.size()); elem++)
	{
		if (elem == m_selectedItem)
		{
			screen->drawFilledRect(x, y, m_internWidth, textHeight, Color(0.3f, 0.7f, 0.9f));
		}
		x2 = x;
		std::vector<int>::iterator widthIt = m_columnWidths.begin();
		//render columns:
		for (row::iterator colIt = m_table[elem].begin(); colIt != m_table[elem].end(); colIt++, widthIt++)
		{
			screen->drawText(*colIt, x2, y, Color::BLACK);
			x2 += *widthIt;
		}
		y += textHeight;
	}
	
	Widget::draw(screen);
}

void Table::mouseClick(EventMouseClick* event)
{
	if (event->m_button == EventMouseClick::BTN_LEFT)
	{
		if (hasFocus() && event->m_isButtonDown && (event->getRelX(this) <= m_internWidth)) 
		{
			int selectedItem = (event->m_absY - m_absY) / VideoSystem::getInstance()->getDefaultFont()->getTextHeight(" ", Font::SIZE_SMALL) - 1;
			if (selectedItem == -1)	//titles clicked
			{
				int relX = event->getRelX(this);
				int x = 0;
				for (int elem = 0; elem < static_cast<int>(m_columnWidths.size()); elem++)
				{
					if ((relX > x) && (relX < x + m_columnWidths[elem]))
					{
						sortByColumn(elem);
						break;
					}
					x += m_columnWidths[elem];
				}
			}
			else if ((selectedItem >= 0) && (selectedItem < static_cast<signed int>(m_table.size())))
			{
				m_selectedItem = selectedItem + (m_page-1)*m_maxItemsPerPage;
			}
			doCommand();
		}
	}
	else if (event->m_button == EventMouseClick::BTN_WHEELUP)
		m_scrollBarVer->setCount(m_scrollBarVer->getCount()-1);
	else if (event->m_button == EventMouseClick::BTN_WHEELDOWN)
		m_scrollBarVer->setCount(m_scrollBarVer->getCount()+1);

}

void Table::setColumnSize(int column, int size)
{
	m_columnWidths[column] = size;
	int add = 0;
	for (std::vector<int>::iterator it = m_columnWidths.begin(); it != m_columnWidths.end(); it++)
		add += *it;
	m_columnWidths.back() += (m_width - add);
}

void Table::onCommand(EventCommand* event)
{
	if(m_hasScrollBar == true)
	{
		if(event->getSender() == m_scrollBarVer)
		{
			m_page = m_scrollBarVer->getCount();
		}
	}
}

class row_less : public std::binary_function<Table::row, Table::row, bool>
{
	int m_column;
public:
	explicit row_less(int column) : m_column(column) {}
	bool operator() (const Table::row& r1, const Table::row& r2) { return r1[m_column] < r2[m_column]; }
};

void Table::sortByColumn(int column)
{
	if (m_selectedItem >= 0)
	{
		row tmp = m_table[m_selectedItem];
		std::sort(m_table.begin(), m_table.end(), row_less(column));
		int count = 0;
		while (m_table[count] != tmp) 
			count++;
		m_selectedItem = count;
		m_scrollBarVer->setCount((m_selectedItem / m_maxItemsPerPage)+1);
	}
	else
		std::sort(m_table.begin(), m_table.end(), row_less(column));
}

void Table::goDown(int count)
{
	m_page += count;
	if(count < 0 || m_page > m_maxPages)
		m_page = m_maxPages;
}

void Table::goUp(int count)
{
	m_page -= count;
	if(m_page < 1)
		m_page = 1;
}
int Table::getPage()
{
	return m_page;
}

int Table::getMaxPages()
{
	return m_maxPages;
}

void Table::addRow(const row& data)
{
	m_table.push_back(data);
	m_maxPages = ((int) m_table.size() - 1) / m_maxItemsPerPage + 1;
	m_scrollBarVer->setMaxCounts(m_maxPages);
}

class row_cmp_column : public std::unary_function<Table::row, bool>
{
	std::string m_item;
	int m_column;
public:
	explicit row_cmp_column(const std::string& item, int column): m_item(item), m_column(column) {}
	bool operator() (const Table::row& r) { return r[m_column] == m_item; }
};

Table::row& Table::getRowByItem(std::string& item, int column)
{
	rows::iterator it = find_if(m_table.begin(), m_table.end(), row_cmp_column(item, column));
	if (it == m_table.end())
		throw std::exception();
	return *it;
}

Table::row& Table::getSelectedRow()
{
	if ((m_selectedItem < 0) || (m_selectedItem >= static_cast<int>(m_table.size())))
		throw std::exception();
	return m_table[m_selectedItem];
}
		
