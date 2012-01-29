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

#include "gui/teamdialog.h"
#include "gui/widgetmanager.h"
#include "gui/statictext.h"
#include <string>
#include <vector>

TeamDialog::TeamDialog(int x, int y, int width, int height, const std::string& background) : Dialog(x, y, width, height, background, 0)
{
	m_Ok = new ImageButton(0, 0, "Ok", "images/menu/start.png", "images/menu/start_h.png", 0);
	m_Ok->setRelPos((m_width/2)-(m_Ok->getWidth()/2), m_height-m_Ok->getHeight()*2);
	addChild(m_Ok);

	typedef std::vector< std::vector<std::string> > teamlist;
	teamlist team(7);

	team[0].push_back("Project leader");
	team[0].push_back("Thomas Kowaliczek");

	team[1].push_back("Story/Game Designer");
	team[1].push_back("Andre Reichelt");

	team[2].push_back("Programmers:");
	team[2].push_back("Fabian >>Foerbyratte<<");
	team[2].push_back("Lars-Peter Clausen");
	team[2].push_back("Alexander Couzens");
	team[2].push_back("Philipp Kerling");
	team[2].push_back("Dirk Rathlev");
	team[2].push_back("Sven Rech");
	team[2].push_back("Stefan Geiger");
	team[2].push_back("Kristoffer Janke");
	
	team[3].push_back("GP2X Porter:");
	team[3].push_back("Lars-Peter Clausen");

	team[4].push_back("MAC Porter:");
	team[4].push_back(">>Below<<");

	team[5].push_back("Graphic:");
	team[5].push_back("Ben Suttor");
	team[5].push_back("Markus Lohaus");
	team[5].push_back("Stefan Herndlbauer");
	team[5].push_back("Waldemar Koch");

	team[6].push_back("Sound:");
	team[6].push_back("Sebastian Ahlen");

	int yPos = 40;
	int xPos = 40;
	StaticText* temp;
	
	for (teamlist::iterator teamIt = team.begin(); teamIt != team.end(); teamIt++)
	{
		temp = new StaticText(xPos, yPos, teamIt->front());
		yPos += temp->getHeight() + 7;
		addChild(temp);
		for (std::vector<std::string>::iterator strIt = ++teamIt->begin(); strIt != teamIt->end(); strIt++)
		{
			temp = new StaticText(xPos+20, yPos, *strIt);
			yPos += temp->getHeight() + 5;
			addChild(temp);
		}
		yPos += temp->getHeight() + 5;
		if ((yPos > m_height*0.8) && (xPos < m_width/2))
		{
			yPos = 40;
			xPos = m_width/2+50;
		}
	}

	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<TeamDialog, EventCommand>(this, &TeamDialog::onCommand));
}

void TeamDialog::onCommand(EventCommand* event)
{
	if (event->getSender() == m_Ok)
		doCommand();
}
