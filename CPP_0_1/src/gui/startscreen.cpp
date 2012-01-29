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
#include <sstream>
#include "startscreen.h"
#include "events/eventmousemove.h"
#include "gui/widgetmanager.h"
#include "openanno.h"
#include "configuration.h"
#include "gettext.h"
#include "ingame/ingame.h"
//#include "messagebox.h"

StartScreen::StartScreen(int x, int y, OpenAnno* app) : Widget(x, y), GameState(app), m_background(NULL), m_backgroundScale(1.0f), m_startMenu(NULL)
{
	VideoSystem* screen = VideoSystem::getInstance();
	m_width = screen->getWidth();
	m_height = screen->getHeight();
	m_startMenu = new Menu(0, 0, "images/menu/background.png");
	m_startMenu->setItemSpacing(10);
	if((m_width - m_startMenu->getWidth()) > 0 && (m_height - m_startMenu->getHeight()) > 0)
	{
		m_startMenu->setRelPos((m_width - m_startMenu->getWidth())/2, (m_height - m_startMenu->getHeight())/2);
	}

	m_startMenu->addItem(_("Start Singleplayer"), "images/menu/start.png", "images/menu/start_h.png", ID_SINGLE);
	m_startMenu->addItem(_("Start Multiplayer"), "images/menu/start.png", "images/menu/start_h.png", ID_MULTI);
	m_startMenu->addItem(_("Configuration"), "images/menu/settings.png", "images/menu/settings_h.png", ID_CONFIG);
	m_startMenu->addItem(_("Team"), "images/menu/team.png", "images/menu/team_h.png", ID_TEAM);
 	m_startMenu->addItem(_("Exit"), "images/menu/quit.png", "images/menu/quit_h.png", ID_EXIT);

	addChild(m_startMenu);
	pushDialog(m_startMenu);

	m_background = VideoSystem::getInstance()->createSurface("images/menu/menu_bg.png");
	if(m_background.isNotNull())
	{
		m_backgroundScale = m_width / static_cast<float>(m_background->getWidth());
	}


	m_singleDialog = new SingleDialog(0, 0, -1, -1, "images/menu/background.png", ID_NEWGAME);
	m_singleDialog->hide();
	m_singleDialog->setRelPos((m_width - m_singleDialog->getWidth())/2, (m_height - m_singleDialog->getHeight())/2);
	addChild(m_singleDialog);

	m_multiDialog = new MultiDialog(0, 0, -1, -1, "images/menu/background.png", ID_SEARCH, ID_LOGIN, ID_REGISTER);
	m_multiDialog->hide();
	m_multiDialog->setRelPos((m_width - m_multiDialog->getWidth())/2, (m_height - m_multiDialog->getHeight())/2);
	addChild(m_multiDialog);

	m_configDialog = new ConfigDialog(0, 0, -1, -1, "images/menu/background.png");
	m_configDialog->hide();
	m_configDialog->setRelPos((m_width - m_configDialog->getWidth())/2, (m_height - m_configDialog->getHeight())/2);
	addChild(m_configDialog);

	m_teamDialog = new TeamDialog(0, 0, -1, -1, "images/menu/background.png");
	m_teamDialog->hide();
	m_teamDialog->setRelPos((m_width - m_teamDialog->getWidth())/2, (m_height - m_teamDialog->getHeight())/2);
	addChild(m_teamDialog);

	m_searchGames = new SearchGames(0, 0, -1, -1, "images/menu/background.png");
	m_searchGames->hide();
	m_searchGames->setRelPos((m_width - m_searchGames->getWidth())/2, (m_height - m_searchGames->getHeight())/2);
	addChild(m_searchGames);

	m_newGameDialog = new NewGameDialog(0, 0, -1, -1, "images/menu/background.png", ID_INGAME);
	m_newGameDialog->hide();
	m_newGameDialog->setRelPos((m_width - m_newGameDialog->getWidth())/2, (m_height - m_newGameDialog->getHeight())/2);
	addChild(m_newGameDialog);

	m_loginDialog = new LoginDialog(0, 0, -1, -1, "images/menu/background.png");
	m_loginDialog->hide();
	m_loginDialog->setRelPos((m_width - m_loginDialog->getWidth())/2, (m_height - m_loginDialog->getHeight())/2);
	addChild(m_loginDialog);

	m_registerDialog = new RegisterDialog(0, 0, -1, -1, "images/menu/background.png");
	m_registerDialog->hide();
	m_registerDialog->setRelPos((m_width - m_registerDialog->getWidth())/2, (m_height - m_registerDialog->getHeight())/2);
	addChild(m_registerDialog);

	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<StartScreen, EventCommand>(this, &StartScreen::startMenuCommand));
	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<StartScreen, EventKey>(this, &StartScreen::keyPress));
}


StartScreen::~StartScreen()
{
	while(m_dialogStack.size() > 0)
	{
		popDialog();
	}
}

void StartScreen::draw(const VideoSystem* const screen)
{
	if(m_background.isNotNull())
	{
		screen->drawSurfaceScaled(m_background, m_absX, m_absY, m_backgroundScale);
	}

	Widget::draw(screen);

	if(Config::getInt("showfps"))
	{
		std::ostringstream fps;
		fps << "FPS: " << m_app->getFPS();
		screen->drawText(fps.str(), m_width-100, 5, Color::WHITE, Font::SIZE_SMALL);
	}

}

void StartScreen::startMenuCommand(EventCommand* event)
{
	if(event->getSender() == m_startMenu)
	{
		if(event->m_id == ID_SINGLE)
		{
			pushDialog(m_singleDialog);
		}
		else if(event->m_id == ID_MULTI)
		{
			pushDialog(m_multiDialog);
		}
		else if(event->m_id == ID_CONFIG)
		{
			pushDialog(m_configDialog);
		}
		else if (event->m_id == ID_TEAM)
		{
			pushDialog(m_teamDialog);
		}

		else if(event->m_id == ID_EXIT)
		{
			m_app->requestExit();
		}
	}
	else if(event->getSender() == m_singleDialog)
	{
		if(event->m_id == ID_NEWGAME)
		{
			m_newGameDialog->showCampaign(m_singleDialog->getSelectedFile());
			pushDialog(m_newGameDialog);
		}
		else
			popDialog();
	}
	else if(event->getSender() == m_multiDialog)
	{
		if(event->m_id == ID_SEARCH)
			pushDialog(m_searchGames);
		else if(event->m_id == ID_LOGIN)
			pushDialog(m_loginDialog);
		else if(event->m_id == ID_REGISTER)
			pushDialog(m_registerDialog);
		else
			popDialog();
	}
	else if(event->getSender() == m_configDialog)
	{
		popDialog();
	}
	else if(event->getSender() == m_teamDialog)
	{
		popDialog();
	}

	else if(event->getSender() == m_searchGames)
	{
		popDialog();
	}
	else if(event->getSender() == m_newGameDialog)
	{
		if(event->m_id == ID_INGAME)
		{
			Ingame* ingame = new Ingame(m_app);
			m_app->pushGameState(ingame);
			ingame->loadMission(m_singleDialog->getSelectedFile());
		}
		else
			popDialog();
	}
/*	else if(event->m_id == Messagebox::OK)
	{
		m_app->exit();
	}*/
}

void StartScreen::update(float delay)
{
	if(VideoSystem::getInstance()->getWidth() != m_width || VideoSystem::getInstance()->getHeight() != m_height)
	{
		m_width = VideoSystem::getInstance()->getWidth();
		m_height = VideoSystem::getInstance()->getHeight();

		if((m_width - m_startMenu->getWidth()) > 0 || (m_height - m_startMenu->getHeight()) > 0)
		{
			m_startMenu->setRelPos((m_width - m_startMenu->getWidth())/2, (m_height - m_startMenu->getHeight())/2);
		}
		else
		{
			m_startMenu->setRelPos(0, 0);
		}
		m_singleDialog->setRelPos((m_width - m_singleDialog->getWidth())/2, (m_height - m_singleDialog->getHeight())/2);
		m_multiDialog->setRelPos((m_width - m_multiDialog->getWidth())/2, (m_height - m_multiDialog->getHeight())/2);
		m_configDialog->setRelPos((m_width - m_configDialog->getWidth())/2, (m_height - m_configDialog->getHeight())/2);
		m_teamDialog->setRelPos((m_width - m_teamDialog->getWidth())/2, (m_height - m_teamDialog->getHeight())/2);

		m_searchGames->setRelPos((m_width - m_searchGames->getWidth())/2, (m_height - m_searchGames->getHeight())/2);
		m_newGameDialog->setRelPos((m_width - m_newGameDialog->getWidth())/2, (m_height - m_newGameDialog->getHeight())/2);
		if(m_background.isNotNull())
		{
			m_backgroundScale = m_width / static_cast<float>(m_background->getWidth());
		}
	}
	VideoSystem::getInstance()->clear();
	WidgetManager::getInstance()->draw();
	VideoSystem::getInstance()->update();
}

bool StartScreen::requestExit()
{
//	disable();
//	new Messagebox(-1, -1, "Do you want to quit Openanno?", Messagebox::YESNO);
	return true;
}

void StartScreen::pushDialog(Dialog* dialog)
{
	if(!m_dialogStack.empty())
	{
		m_dialogStack.top()->hide();
	}
	m_dialogStack.push(dialog);
	m_dialogStack.top()->show();
	WidgetManager::getInstance()->updateWidgetUnderMouse();
}

void StartScreen::popDialog()
{
	m_dialogStack.top()->hide();
	m_dialogStack.pop();
	if(!m_dialogStack.empty())
	{
		m_dialogStack.top()->show();
	}
	WidgetManager::getInstance()->updateWidgetUnderMouse();
}

void StartScreen::keyPress(EventKey* event)
{
	if(event->m_keyCode == SDLK_ESCAPE && !event->m_isKeyDown && m_isEnabled)
	{
		if(m_dialogStack.size() > 1)
		{
				popDialog();
		}
		else
		{
			m_app->requestExit();
		}
	}
}

void StartScreen::activate()
{
	// Close all open dialogs and show startscreen
	while(m_dialogStack.size() > 1)
	{
		popDialog();
	}

	show();
	enable();
}

void StartScreen::deactivate()
{
	hide();
	disable();
}
