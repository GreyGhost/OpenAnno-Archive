/***************************************************************************
 *   Copyright (C) 2006 by Alexander Couzens							   *
 *   acouzens@gmx.net                                                      *
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

#ifndef STARTDIALOG_H_
#define STARTDIALOG_H_

#include <string>
#include "imagebutton.h"
#include "gui/dialog.h"
#include "gui/events/eventcommand.h"
#include "gui/selectbox.h"

class SingleDialog : public Dialog
{
public:
	SingleDialog(int x, int y, int width, int height, const std::string& background, int newGameCommId);
	std::string& getSelectedFile();
private:
	void onCommand(EventCommand* event);
	void addMissions();
	ImageButton* m_newgame;
	ImageButton* m_loadgame;
	ImageButton* m_Cancel;
	SelectBox* m_missions;
	int m_newGameCommandId;
};

#endif /*SingleDialog_H_*/






