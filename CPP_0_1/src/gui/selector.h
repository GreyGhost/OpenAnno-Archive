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

#ifndef SELECTOR_H
#define SELECTOR_H

#include <string>
#include <vector>
#include "video/surface.h"
#include "video/videosystem.h"
#include "gui/commandwidget.h"
#include "gui/events/eventmouseclick.h"

class Selector : public CommandWidget
{
public:
	enum Type
	{
		VERTICAL_SELECTOR,
		HORIZONTAL_SELECTOR
	};

	Selector(int x, int y, Type type, int commandId = -1);
	virtual ~Selector();
	virtual void draw(const VideoSystem* const screen);
	virtual void addItem(const std::string& title, const std::string& name = "");
	std::string getSelectedItemTitle() const;
	std::string getSelectedItemName() const;
	int getSelectedItemIndex() const;
	virtual void mouseClick(EventMouseClick* event);
	void selectItem(int index);
	bool empty();

protected:
	Type m_type;
	std::vector<std::string> m_itemsTitle;
	std::vector<std::string> m_itemsName;
	int m_selectedItem;
	SurfacePtr m_arrows[2];
};


#endif
