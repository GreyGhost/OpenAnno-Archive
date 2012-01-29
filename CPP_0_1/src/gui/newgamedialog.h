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

#ifndef NEWGAMEDIALOG_H_
#define NEWGAMEDIALOG_H_

#include "gui/dialog.h"
#include "gui/imagebutton.h"
#include "gui/events/eventcommand.h"
#include "gui/events/eventkey.h"
#include "gui/textviewer.h"

class NewGameDialog : public Dialog
{
public:
	NewGameDialog(int x, int y, int width, int height, const std::string& background, int ingameCommId);
	void showCampaign(const std::string& file);
private:
	void onCommand(EventCommand* event);
	ImageButton* m_startGame;
	ImageButton* m_Cancel;
	int m_ingameCommandId;
	TextViewer* m_missionViewer;
};

#endif
