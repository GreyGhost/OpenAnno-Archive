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

#include "singledialog.h"
#include "widgetmanager.h"

#include "gettext.h"

#include <sstream>

#include <stdlib.h>
#include <iostream>

#include <physfs.h>
#include "lib/tinyxml/tinyxml.h"

SingleDialog::SingleDialog(int x, int y, int width, int height, const std::string& background, int newGameCommId) : Dialog(x, y, width, height, background, 0), m_newGameCommandId(newGameCommId)
{
	int pX = 50, pY = 50;
	m_newgame = new ImageButton(pX, pY, _("Start New Game"), "images/menu/start.png", "images/menu/start_h.png", 0);
	addChild(m_newgame);
	pY += m_newgame->getHeight() + 10;

	m_loadgame = new ImageButton(pX, pY, _("Load Game"), "images/menu/start.png", "images/menu/start_h.png", 0);
	addChild(m_loadgame);
	pY += m_loadgame->getHeight() + 10;
	
	m_Cancel = new ImageButton(pX, pY, _("Back"), "images/menu/quit.png", "images/menu/quit_h.png", 0);
	addChild(m_Cancel);
	

	pX = 50 + m_newgame->getWidth() + 15;
	pY = 50;
	m_missions = new SelectBox(pX, pY, m_width-pX-35, 300, true, 0);
	addChild(m_missions);
	addMissions();


	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<SingleDialog, EventCommand>(this, &SingleDialog::onCommand));
}

void SingleDialog::onCommand(EventCommand* event)
{
	if (event->getSender() == m_newgame)
	{
		if (!m_missions->empty())
			sendEvent(new EventCommand(m_newGameCommandId));
	}
	else if(event->getSender() == m_Cancel)
	{
		doCommand();
	}
}

void SingleDialog::addMissions()
{
	std::string path = "xml/missions/";
	char** files = PHYSFS_enumerateFiles(path.c_str());
	TiXmlDocument xml;
	for (char** it = files; *it != NULL; it++)
	{
		if (xml.PHYSFS_LoadFile(path + *it))
			if (xml.RootElement()->ValueStr() == "mission")
			{
				TiXmlElement* element = xml.RootElement()->FirstChildElement("info");
				if (element)
				{
					element = element->FirstChildElement("name");
					if (element)
						m_missions->addItem(element->FirstChild()->ValueStr(), path + *it);
				}
			}
	}
	PHYSFS_freeList(files);
}

std::string& SingleDialog::getSelectedFile()
{
	return m_missions->getSelected();
}
