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

#ifndef MENU_H
#define MENU_H

#include <string>
#include "video/videosystem.h"
#include "video/surface.h"
#include "gui/widget.h"
#include "gui/events/eventmousemove.h"
#include "gui/events/eventmouseclick.h"
#include "dialog.h"

class MenuItem : public Widget
{
public:
	MenuItem(int x, int y, const std::string& title, const std::string& icon, const std::string& iconHighlited, int id);
	~MenuItem();
	void draw(const VideoSystem* const screen);
	int getId() const;

private:
	SurfacePtr m_icon;
	SurfacePtr m_iconHighlighted;
	std::string m_title;
	int m_textOffsetX, m_textOffsetY;
	int m_id;
};

class Menu : public Dialog
{
public:
	Menu(int x, int y, const std::string& background);
	 ~Menu();
	void mouseClick(EventMouseClick* event);
	void mouseMove(EventMouseMove* event);

	void addItem(const std::string& title, const std::string& icon, const std::string& iconHighlited, int commandId);
	void setItemSpacing(int spacing) { m_itemSpaceing = spacing; };

protected:
	int m_itemSpaceing;
};


#endif
