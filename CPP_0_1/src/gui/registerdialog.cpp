/***************************************************************************
 *   Copyright (C) 2006 by Marc Engelhardt								   *
 *   Marc_Engelhardt@gmx.net                                               *
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

#include <string>
#include "gettext.h"
#include "gui/registerdialog.h"
#include "configuration.h"
#include "gui/dialog.h"
#include "gui/widgetmanager.h"
#include "gui/statictext.h"
#include "gui/events/eventcommand.h"

RegisterDialog::RegisterDialog(int x, int y, int width, int height, const std::string& background) : Dialog(x, y, width, height, background, 0)
{
	int pX = 50, pY = 40;
	Widget* temp;
	temp = new StaticText(pX, pY, _("Player Name"));
	addChild(temp);
	pY += temp->getHeight() + 2;

	m_playerName = new Editbox(pX, pY, 150);
	addChild(m_playerName);
	m_playerName->setText(Config::getString("playername"));
	pY += m_playerName->getHeight() + 40;

	temp = new StaticText(pX, pY, _("Player Email"));
	addChild(temp);
	pY += temp->getHeight() + 2;

	m_playerEmail = new Editbox(pX, pY, 150);
	addChild(m_playerEmail);
	m_playerEmail->setText(Config::getString("playeremail"));
	pY += m_playerEmail->getHeight() + 40;

	temp = new StaticText(pX, pY, _("Player Passwort"));
	addChild(temp);
	pY += temp->getHeight() + 2;

	m_playerPasswort = new Editbox(pX, pY, 150);
	addChild(m_playerPasswort);
	m_playerPasswort->setText(Config::getString("playerpasswort"));
	pY += m_playerPasswort->getHeight() + 40;

	temp = new StaticText(pX, pY, _("Player Passwort"));
	addChild(temp);
	pY += temp->getHeight() + 2;

	m_playerPasswortW = new Editbox(pX, pY, 150);
	addChild(m_playerPasswortW);
	m_playerPasswortW->setText(Config::getString("playerpasswortw"));
	pY += m_playerPasswortW->getHeight() + 40;

	m_Ok = new ImageButton(0, 0, _("Ok"), "images/menu/start.png", "images/menu/start_h.png", 0);
   	addChild(m_Ok);
   	pX = (m_width/4 - m_Ok->getWidth())/2 + m_width/4;
   	pY = m_height - m_Ok->getHeight()*2;
   	m_Ok->setRelPos(pX, pY);
   
   	m_Cancel = new ImageButton(pX, pY, _("Cancel"), "images/menu/quit.png", "images/menu/quit_h.png", 0);
   	addChild(m_Cancel);
   	pX = (m_width/4 - m_Cancel->getWidth())/2 + m_width/2;
   	pY = m_height - m_Cancel->getHeight()*2;
   	m_Cancel->setRelPos(pX, pY);
}

void RegisterDialog::onCommand(EventCommand* event)
{
	if (event->getSender() == m_Ok)
		doCommand();
	if (event->getSender() == m_Cancel)
		doCommand();
}
