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

#include "gui/searchgames.h"
#include "gui/widgetmanager.h"
#include "gettext.h"

#include <cstdlib> //Nur für die Zufallszahlen
#include <sstream>

SearchGames::SearchGames(int x, int y, int width, int height, const std::string& background) : Dialog(x, y, width, height, background, 0)
{
	int pX = 50, pY = 50;
	m_refresh = new ImageButton(pX, pY, _("Refresh"), "images/menu/start.png", "images/menu/start_h.png", 0);
	addChild(m_refresh);
	pY += m_refresh->getHeight()+10;
	
	m_connect = new ImageButton(pX, pY, _("Connect"), "images/menu/start.png", "images/menu/start_h.png", 0);
	addChild(m_connect);
	pY += m_connect->getHeight()+10;
	
	m_connectip = new ImageButton(pX, pY, _("Connect IP"), "images/menu/start.png", "images/menu/start_h.png", 0);
	addChild(m_connectip);
	pY += m_connectip->getHeight()+10;
	
	m_Ok = new ImageButton(0, 0, _("Ok"), "images/menu/start.png", "images/menu/start_h.png", 0);
	addChild(m_Ok);
	pX = (m_width - m_Ok->getWidth())/2;
	pY = m_height - m_Ok->getHeight()*2;
	m_Ok->setRelPos(pX, pY); 

	pX = 390;
	pY = 50;
	m_lan = new Checkbox(pX, pY, "Lan", 0);
	m_lan->check();
	addChild(m_lan);
	pX += m_lan->getWidth() + 15;
	m_internet = new Checkbox(pX, pY, "Internet", 0);
	addChild(m_internet);
	pX += m_internet->getWidth() + 5;
	pY += m_internet->getHeight() + 5;

	Table::row dummy(3, "Titel");
	m_table = new Table(180, pY, pX-180, 280, dummy, true);
	addChild(m_table);
	//Zum Test ein paar Zufallszahlen reinschreiben:
	for (int x = 0; x < 50; x++)
	{
		Table::row dummy;
		std::stringstream dummy2;
		dummy2 << std::rand()%100;
		dummy.push_back(dummy2.str());
		dummy2.str("");
		dummy2 << std::rand()%100;
		dummy.push_back(dummy2.str());
		dummy2.str("");
		dummy2 << std::rand()%100;
		dummy.push_back(dummy2.str());
		m_table->addRow(dummy);
	}

	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<SearchGames, EventCommand>(this, &SearchGames::onCommand));
}

void SearchGames::onCommand(EventCommand* event)
{
	if (event->getSender() == m_Ok)
		doCommand();
	else if(event->getSender() == m_lan)
	{
		if(m_lan->isChecked() == true)
			m_internet->check(false);
		else
			m_internet->check(true);
	}
	else if(event->getSender() == m_internet)
	{
		if(m_internet->isChecked() == true)
			m_lan->check(false);
		else
			m_lan->check(true); 
	}
}
