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

#ifndef INGAME_H
#define INGAME_H

#include <string>
#include "gamestate.h"
#include "gui/widget.h"
#include "gui/imagebutton.h"
#include "openanno.h"
#include "map.h"
#include "minimap.h"
#include "mission.h"
#include "video/videosystem.h"
#include "video/surface.h"

#include "gui/events/eventmousemove.h"
#include "gui/widgetmanager.h"

class Ingame : public GameState, public Widget
{
public:
	Ingame(OpenAnno* app);
	~Ingame();
	void loadMission(const std::string& filename);
	bool requestExit();
	void update(float delay);
	void draw(const VideoSystem* const screen);
	void activate();
	void deactivate();
	void keyPress(EventKey* event);
private:
	void onMouseMove(EventMouseMove* event);
	void onCommand(EventCommand* event);

	Map* m_map;
	MiniMap* m_miniMap;
	Mission* m_mission;
	ImageButton* m_rotateLeftButton;
	ImageButton* m_rotateRightButton;
};

#endif
