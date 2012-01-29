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

#include "gettext.h"
#include "gui/startscreen.h"
#include "gui/multidialog.h"
#include "gui/widgetmanager.h"

MultiDialog::MultiDialog(int x, int y, int width, int height, const std::string& background, int searchCommId, int loginCommId, int registerCommId) : Dialog(x, y, width, height, background, 0), m_loginCommandId(loginCommId), m_registerCommandId(registerCommId), m_searchCommandId(searchCommId)
{
	int pX = 50, pY = 50;
	m_newsession = new ImageButton(pX, pY, _("New Mission"), "images/menu/start.png", "images/menu/start_h.png", 0);
   	addChild(m_newsession);
   	pY += m_newsession->getHeight() + 10;

    m_loadsession = new ImageButton(pX, pY, _("Load Session"), "images/menu/start.png", "images/menu/start_h.png", 0);
   	addChild(m_loadsession);
   	pY += m_loadsession->getHeight() + 10;

   	m_searchgames = new ImageButton(pX, pY, _("Search Games"), "images/menu/start.png", "images/menu/start_h.png", 0);
	addChild(m_searchgames);
   	pY += m_searchgames->getHeight() + 10;

	m_logindialog = new ImageButton(pX, pY, _("Login"), "images/menu/start.png", "images/menu/start_h.png", 0);
	addChild(m_logindialog);
   	pY += m_logindialog->getHeight() + 10;

	m_registerdialog = new ImageButton(pX, pY, _("Register"), "images/menu/start.png", "images/menu/start_h.png", 0);
	addChild(m_registerdialog);
   	pY += m_registerdialog->getHeight() + 10;

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

		WidgetManager::getInstance()->addEventListener(new SpecificEventListener<MultiDialog, EventCommand>(this, &MultiDialog::onCommand));
}

MultiDialog::~MultiDialog()
{
}

void MultiDialog::onCommand(EventCommand* event)
{
	if(event->getSender() == m_Ok)
	{
		doCommand();
	}
	else if(event->getSender() == m_Cancel)
	{
		doCommand();
	}
	else if(event->getSender() == m_searchgames)
	{
		sendEvent(new EventCommand(m_searchCommandId));
	}
	if(event->getSender() == m_logindialog)
	{
		sendEvent(new EventCommand(m_loginCommandId));
	}
	else if(event->getSender() == m_registerdialog)
	{
		sendEvent(new EventCommand(m_registerCommandId));
	}
}
