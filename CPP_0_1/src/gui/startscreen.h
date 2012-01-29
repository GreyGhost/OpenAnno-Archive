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

#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <stack>

#include "video/videosystem.h"
#include "video/surface.h"
#include "gui/widget.h"
#include "gui/menu.h"
#include "gui/events/eventcommand.h"
#include "gui/checkbox.h"
#include "gui/slider.h"
#include "gui/statictext.h"
#include "gui/selector.h"
#include "gui/editbox.h"
#include "gui/dialog.h"
#include "gamestate.h"

#include "gui/singledialog.h"
#include "gui/multidialog.h"
#include "gui/configdialog.h"
#include "gui/teamdialog.h"
#include "gui/searchgames.h"
#include "gui/newgamedialog.h"
#include "gui/logindialog.h"
#include "gui/registerdialog.h"

class OpenAnno;

class StartScreen : public Widget, public GameState
{
public:
	StartScreen(int _x, int _y, OpenAnno* m_app);
	 ~StartScreen();
	void draw(const VideoSystem* const screen);
	bool requestExit();
	void update(float delay);
	void activate();
	void deactivate();

protected:
	void keyPress(EventKey* event);
	void startMenuCommand(EventCommand* event);
	void pushDialog(Dialog* dialog);
	void popDialog();

	SurfacePtr m_background;
	float m_backgroundScale;
	Menu* m_startMenu;
	SingleDialog* m_singleDialog;
	MultiDialog* m_multiDialog;
	ConfigDialog* m_configDialog;
	TeamDialog* m_teamDialog;
	SearchGames* m_searchGames;
	NewGameDialog* m_newGameDialog;
	LoginDialog* m_loginDialog;
	RegisterDialog* m_registerDialog;

	std::stack<Dialog*> m_dialogStack;

	Checkbox* checkbox;
	Slider* slider;
	Editbox* editbox;
	Selector* selector[2];
	StaticText* staticText;


private:
	enum
	{
		ID_SINGLE,
		ID_MULTI,
		ID_CONFIG,
		ID_TEAM,
		ID_EXIT,
		ID_SEARCH,
		ID_NEWGAME,
		ID_INGAME,
		ID_LOGIN,
		ID_REGISTER
	};
};


#endif
