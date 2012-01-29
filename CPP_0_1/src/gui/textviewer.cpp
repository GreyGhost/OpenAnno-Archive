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

#include "gui/textviewer.h"
#include "gui/widgetmanager.h"

#include <iostream>

TextViewer::TextViewer(int x, int y, int width, int height, std::string text, bool hasScrollBar) : Widget(x, y), m_hasScrollBar(hasScrollBar)
{
	m_width = width;
	m_height = height;

	m_maxItemsPerPage = (m_height / VideoSystem::getInstance()->getDefaultFont()->getTextHeight(" ", Font::SIZE_SMALL));
	m_maxPages = 1;
	m_page = 1;

	if (hasScrollBar)
	{
		m_scrollbar = new ScrollBar(x, y, height, 1, ScrollBar::VERTICAL, 0);
		addChild(m_scrollbar);
		m_scrollbar->setRelPos(width - m_scrollbar->getWidth(), 0);
		m_scrollbar->setMaxCounts(m_maxPages);
		m_internWidth = m_width - m_scrollbar->getWidth();

		WidgetManager::getInstance()->addEventListener(new SpecificEventListener<TextViewer, EventCommand>(this, &TextViewer::onCommand));
	} else
		m_internWidth = m_width;

	if (text != "")
		setText(text);
}

void TextViewer::setText(std::string text)
{
	m_textList.clear();
	std::string::iterator begin = text.begin();
	std::string::iterator potEnd = begin;
	for (std::string::iterator it = text.begin(); it != text.end(); it++)
	{
		if ((*it == ' ') || (*it == '\n'))
		{
			int length = VideoSystem::getInstance()->getDefaultFont()->getTextWidth(std::string(begin, it), Font::SIZE_MIDDLE);
			if (length > m_internWidth)
			{
				if (potEnd <= begin)
					potEnd = it;
				m_textList.push_back(std::string(begin, potEnd));
				begin = potEnd+1;
			}
			if ((begin <= it) && (*it == '\n'))
			{
				m_textList.push_back(std::string(begin, it));
				begin = it+1;
			}
			potEnd = it;
		}
	}
	if (begin != text.end())
		m_textList.push_back(std::string(begin, text.end()));

	m_maxPages = ((int) m_textList.size() - 1) / m_maxItemsPerPage + 1;
	m_scrollbar->setMaxCounts(m_maxPages);
	m_page = 1;
}

void TextViewer::onCommand(EventCommand* event)
{
	if (m_hasScrollBar)
		if (event->getSender() == m_scrollbar)
			m_page = m_scrollbar->getCount();
}

void TextViewer::draw(const VideoSystem* const screen)
{
	int textHeight = screen->getDefaultFont()->getTextHeight(" ", Font::SIZE_SMALL);
	int first = m_maxItemsPerPage * (m_page-1);
	int last = first + m_maxItemsPerPage-1;
	if (last >= (int)m_textList.size())
		last = (int)m_textList.size()-1;
	int y = m_absY;
	for (int c = first; c <= last; c++)
	{
		screen->drawText(m_textList[c], m_absX, y, Color::BLACK);
		y += textHeight;
	}
	Widget::draw(screen);
}
