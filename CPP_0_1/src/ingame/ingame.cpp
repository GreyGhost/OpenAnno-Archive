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

#include "lib/tinyxml/tinyxml.h"
#include "ingame.h"
#include "configuration.h"

#include <sstream>

Ingame::Ingame(OpenAnno* app) : GameState(app), Widget(0, 0)
{
	m_width = VideoSystem::getInstance()->getWidth();
	m_height = VideoSystem::getInstance()->getHeight();

	m_map = new Map(0, 0, m_width, m_height);
	m_miniMap = new MiniMap(m_width, m_height, m_map, 123456);
	m_mission = new Mission(m_map, m_miniMap);
	addChild(m_map);
	addChild(m_miniMap);

	m_rotateLeftButton = new ImageButton(m_width - 150, 30, "", "images/hud/rotate_left.png", "images/hud/rotate_left_h.png", 0);
	addChild(m_rotateLeftButton);
	m_rotateRightButton = new ImageButton(m_width - 150 + 64, 30, "", "images/hud/rotate_right.png", "images/hud/rotate_right_h.png", 0);
	addChild(m_rotateRightButton);

	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<Ingame, EventCommand>(this, &Ingame::onCommand));
	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<Ingame, EventKey>(this, &Ingame::keyPress));
}

Ingame::~Ingame()
{
	delete m_mission;
	m_mission = NULL;
}

bool Ingame::requestExit()
{
	return true;
}

void Ingame::draw(const VideoSystem* const screen)
{
	Widget::draw(screen);

	if(Config::getInt("showfps"))
	{
		std::ostringstream fps;
		fps << "FPS: " << m_app->getFPS();
		screen->drawText(fps.str(), m_width-100, 5, Color::WHITE, Font::SIZE_SMALL);
	}
}

void Ingame::update(float delay)
{
	assert (m_mission && m_map && m_miniMap);

	if (m_mission->objectCompleted())
	{
		//TODO: show a "finished" dialog or something
		return;
	}

	const int mouseX = WidgetManager::getInstance()->getMouseX();
	const int mouseY = WidgetManager::getInstance()->getMouseY();

	int scrollX = 0, scrollY = 0;

	const static int tol = 25;
	float scrollSpeed = (Config::getFloat("scroll_speed") * 5.0f + 10.0f) * delay * 10.0f; // This gives a value betwen 150 and 600 pixel per second

	if(scrollSpeed < 1)
	{
		scrollSpeed = 1;
	}
	if (mouseX < tol)
	{
		scrollX = static_cast<int>(-scrollSpeed*(-mouseX + tol)) / 10;
	}
	else if (mouseX > m_width-tol)
	{
		scrollX = static_cast<int>(scrollSpeed*(mouseX - (m_width - tol))) / 10;
	}
	if (mouseY < tol)
	{
		scrollY = static_cast<int>(-scrollSpeed*(-mouseY + tol)) / 10;
	}
	else if (mouseY > m_height-tol)
	{
		scrollY = static_cast<int>(scrollSpeed*(mouseY - (m_height - tol))) / 10;
	}
	m_map->scroll(scrollX, scrollY);

	m_map->update(delay);

	VideoSystem::getInstance()->clear();
	WidgetManager::getInstance()->draw();
	VideoSystem::getInstance()->update();
}

void Ingame::loadMission(const std::string& filename)
{
	m_mission->load(filename);
}

void Ingame::activate()
{
	show();
}

void Ingame::deactivate()
{
	hide();
}

void Ingame::onCommand(EventCommand* event)
{
	if (event->getSender() == m_rotateLeftButton)
	{
		m_map->rotateLeft();
	}
	else if (event->getSender() == m_rotateRightButton)
	{
		 m_map->rotateRight();
	}
}

void Ingame::keyPress(EventKey* event)
{
	if(event->m_keyCode == SDLK_ESCAPE && !event->m_isKeyDown && m_isEnabled)
	{
		delete m_app->popGameState();
	}
}
