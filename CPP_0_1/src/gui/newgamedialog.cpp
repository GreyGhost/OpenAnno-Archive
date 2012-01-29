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

#include "gui/newgamedialog.h"
#include "gui/widgetmanager.h"

#include "lib/tinyxml/tinyxml.h"
#include "gettext.h"

NewGameDialog::NewGameDialog(int x, int y, int width, int height, const std::string& background, int ingameCommId) : Dialog(x, y, width, height, background, 0), m_ingameCommandId(ingameCommId)
{
	m_missionViewer = new TextViewer(50, 50, 500, 300);
	addChild(m_missionViewer);
	
	m_startGame = new ImageButton(0, 0, _("Start Game"), "images/menu/start.png", "images/menu/start_h.png", 0);
	addChild(m_startGame);
	int pX = (m_width/4 - m_startGame->getWidth())/2 + m_width/4;
	int pY = m_height - m_startGame->getHeight()*2;
	m_startGame->setRelPos(pX, pY); 

	m_Cancel = new ImageButton(pX, pY, _("Cancel"), "images/menu/quit.png", "images/menu/quit_h.png", 0);
	addChild(m_Cancel);
	pX = (m_width/4 - m_Cancel->getWidth())/2 + m_width/2;
	pY = m_height - m_Cancel->getHeight()*2;
	m_Cancel->setRelPos(pX, pY);

	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<NewGameDialog, EventCommand>(this, &NewGameDialog::onCommand));
}

void NewGameDialog::onCommand(EventCommand* event)
{
	if (event->getSender() == m_startGame)
		sendEvent(new EventCommand(m_ingameCommandId));
	else if (event->getSender() == m_Cancel)
		doCommand();
}

void NewGameDialog::showCampaign(const std::string& file)
{
	TiXmlDocument xmldata;
	if (xmldata.PHYSFS_LoadFile(file))
			if (xmldata.RootElement()->ValueStr() == "mission")
			{
				TiXmlElement* element = xmldata.RootElement()->FirstChildElement("info");
				if (element)
				{
					element = element->FirstChildElement("description");
					if (element)
						m_missionViewer->setText(element->FirstChild()->ValueStr());
				}
			}
}
