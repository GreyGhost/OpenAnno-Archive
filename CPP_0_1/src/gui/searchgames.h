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

#ifndef SEARCHGAMES_H_
#define SEARCHGAMES_H_

#include <string>
#include "gui/imagebutton.h"
#include "gui/checkbox.h"
#include "gui/dialog.h"
#include "gui/events/eventcommand.h"
#include "gui/table.h"

class SearchGames : public Dialog
{
public:
	SearchGames(int x, int y, int width, int height, const std::string& background);
private:
	void onCommand(EventCommand* event);
	ImageButton* m_refresh;
	ImageButton* m_connect;
	ImageButton* m_connectip;
	Checkbox* m_lan;
	Checkbox* m_internet;
	ImageButton* m_Ok;
	Table* m_table;
};

#endif
