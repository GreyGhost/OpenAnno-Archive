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

#include "gettext.h"
#include "configdialog.h"
#include "statictext.h"
#include "editbox.h"
#include "checkbox.h"
#include "slider.h"
#include "configuration.h"
#include "widgetmanager.h"
#include <sstream>

#include <stdlib.h>
#include <iostream>
#include <physfs.h>

#include "audio/audioSystem.h"

ConfigDialog::ConfigDialog(int x, int y, int width, int height, const std::string& background) : Dialog(x, y, width, height, background, 0)
{
	m_oldResolution = VideoSystem::getInstance()->getResolution();
	m_oldShowFps = Config::getInt("showfps");
	m_oldRenderer = (Config::getString("renderer") == "opengl");

	int pX = 50, pY = 40;
	Widget* temp;
	temp = new StaticText(pX, pY, _("Player Name"));
	addChild(temp);
	pY += temp->getHeight() + 2;

	m_playerName = new Editbox(pX, pY, 150);
	addChild(m_playerName);
	m_playerName->setText(Config::getString("playername"));
	pY += m_playerName->getHeight() + 40;

	m_showFPS = new Checkbox(pX, pY, _("Show FPS"), 0);
	addChild(m_showFPS);
	if(true/*Config::getInt("showfps")*/)
	{
		m_showFPS->check();
	}
	pY += m_showFPS->getHeight() + 15;

	m_fullscreen = new Checkbox(pX, pY, _("Fullscreen"), 0);
	addChild(m_fullscreen);
	if(Config::getInt("fullscreen"))
	{
		m_fullscreen->check();
	}
	m_oldFullscreen = m_fullscreen->isChecked();
	pY += m_fullscreen->getHeight() + 15;

	m_enableSound = new Checkbox(pX, pY, _("Enable Sound"), 0);
	addChild(m_enableSound);
	if(Config::getInt("sound"))
		m_enableSound->check();
	pY += m_enableSound->getHeight() + 15;

	temp = new StaticText(pX, pY, _("Resolution"));
	addChild(temp);
	pY += temp->getHeight() + 3;

	m_videoModes = new Selector(pX, pY, Selector::HORIZONTAL_SELECTOR);
	addChild(m_videoModes);
	m_videoModeList = VideoSystem::getInstance()->getAvailableModes();
	std::stringstream stream;
	bool foundCurrentMode = false;
	unsigned int i;
	for(i = 0; i < m_videoModeList.size(); i++)
	{
		stream << m_videoModeList[i].getWidth() << "x" << m_videoModeList[i].getHeight() << "x" << m_videoModeList[i].getBpp();
		m_videoModes->addItem(stream.str());
		stream.str("");
		if(m_oldResolution == m_videoModeList[i])
		{
			m_videoModes->selectItem(i);
			foundCurrentMode = true;
		}
	}
	if(!foundCurrentMode)
	{
		stream << m_oldResolution.getWidth() << "x" << m_oldResolution.getHeight() << "x" << m_oldResolution.getBpp();
		m_videoModes->addItem(stream.str());
		m_videoModes->selectItem(i);
		m_videoModeList.push_back(m_oldResolution);
	}
	pY += m_videoModes->getHeight() + 15;

	temp = new StaticText(pX, pY, _("Renderer"));
	addChild(temp);
	pY += temp->getHeight() + 3;
	m_renderer = new Selector(pX, pY, Selector::HORIZONTAL_SELECTOR);
	addChild(m_renderer);

	m_renderer->addItem("SDL");
	m_renderer->addItem("OpenGL");
	if(Config::getString("renderer") == "opengl")
	{
		m_renderer->selectItem(1);
	}

	pX = 300;
	pY = 40;
	temp = new StaticText(pX, pY, _("Scroll Speed"));
	addChild(temp);
	pY += temp->getHeight() + 3;

	m_scrollSpeed = new Slider(pX, pY, 1, 10);
	m_scrollSpeed->setPos(Config::getInt("scroll_speed"));
	addChild(m_scrollSpeed);
	pY += m_scrollSpeed->getHeight() + 15;

	temp = new StaticText(pX, pY, _("Sound Volume"));
	addChild(temp);
	pY += temp->getHeight() + 3;

	m_soundVolume = new Slider(pX, pY, 0, 100, 0);
	m_soundVolume->setPos(static_cast<int>(Config::getFloat("soundvol") * 100.0f));
	addChild(m_soundVolume);
	pY += m_soundVolume->getHeight() + 15;

	temp = new StaticText(pX, pY, _("Music Volume"));
	addChild(temp);
	pY += temp->getHeight() + 3;

	m_musicVolume = new Slider(pX, pY, 0, 100, 0);
	m_musicVolume->setPos(static_cast<int>(Config::getFloat("musicvol") * 100.0f));
	addChild(m_musicVolume);
	pY += m_musicVolume->getHeight() + 15;

	temp = new StaticText(pX, pY, _("Gamma"));
	addChild(temp);
	pY += temp->getHeight() + 3;

	m_gamma = new Slider(pX, pY, 0, 100);
	m_gamma->setPos(static_cast<int>(Config::getFloat("gamma")*100));
	addChild(m_gamma);

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

	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<ConfigDialog, EventCommand>(this, &ConfigDialog::onCommand));
}

ConfigDialog::~ConfigDialog()
{

}

void ConfigDialog::onCommand(EventCommand* event)
{
	if(event->getSender() == m_soundVolume)
	{
		AudioSystem::getInstance()->setGlobalSoundVolume(static_cast<float>(m_soundVolume->getPos()) / 100.0f);
	}
	else if(event->getSender() == m_musicVolume)
	{
		AudioSystem::getInstance()->setGlobalMusicVolume(static_cast<float>(m_musicVolume->getPos()) / 100.0f);
	}
	else if(event->getSender() == m_showFPS)
	{
		Config::setInt("showfps", m_showFPS->isChecked());
	}
	else if(event->getSender() == m_enableSound)
	{
		if (m_enableSound->isChecked())
			AudioSystem::getInstance()->activate();
		else
			AudioSystem::getInstance()->deactivate();
	}
	else if(event->getSender() == m_Ok)
	{
		if(m_oldResolution != m_videoModeList[m_videoModes->getSelectedItemIndex()] || m_oldFullscreen != static_cast<int>(m_fullscreen->isChecked()))
		{
			VideoSystem::getInstance()->reinit(m_videoModeList[m_videoModes->getSelectedItemIndex()].getWidth(), m_videoModeList[m_videoModes->getSelectedItemIndex()].getHeight(), m_videoModeList[m_videoModes->getSelectedItemIndex()].getBpp(), m_fullscreen->isChecked());

			Config::setInt("resolution_x", m_videoModeList[m_videoModes->getSelectedItemIndex()].getWidth());
			Config::setInt("resolution_y", m_videoModeList[m_videoModes->getSelectedItemIndex()].getHeight());
			Config::setInt("depth", m_videoModeList[m_videoModes->getSelectedItemIndex()].getBpp());
			Config::setInt("fullscreen", m_fullscreen->isChecked());
		}
		Config::setString("playername", m_playerName->getText());
		switch(m_renderer->getSelectedItemIndex())
		{
			case 0:
			{
				Config::setString("renderer", "sdl");
				break;
			}
			case 1:
			{
				Config::setString("renderer", "opengl");
				break;
			}
		}
		Config::setInt("scroll_speed", m_scrollSpeed->getPos());
		Config::setFloat("musicvol", m_musicVolume->getPos() / 100.0f);
		Config::setFloat("soundvol", m_soundVolume->getPos() / 100.0f);
		Config::setInt("sound", m_enableSound->isChecked());
        Config::setFloat("gamma",static_cast<float>(m_gamma->getPos()) / 100.0f);

		/*std::string userdir = PHYSFS_getUserDir();
		std::string oadir = userdir + ".openanno";
		std::string configfile = oadir + PHYSFS_getDirSeparator() + "config.xml";*/
		Config::save("config.xml");

		doCommand();
	}
	else if(event->getSender() == m_Cancel)
	{
		Config::setInt("showfps", m_oldShowFps);
		m_fullscreen->check(Config::getInt("fullscreen")>0);
		m_showFPS->check(Config::getInt("showfps")>0);
		for(unsigned int i = 0; i < m_videoModeList.size(); ++i)
		{
			if(m_oldResolution == m_videoModeList[i])
			{
				m_videoModes->selectItem(i);
			}
		}
		m_renderer->selectItem(m_oldRenderer);
		m_scrollSpeed->setPos(Config::getInt("scroll_speed"));
		m_soundVolume->setPos(static_cast<int>(Config::getFloat("soundvol") * 100.0f));
		m_musicVolume->setPos(static_cast<int>(Config::getFloat("musicvol") * 100.0f));

		AudioSystem::getInstance()->setGlobalSoundVolume(Config::getFloat("soundvol"));
		AudioSystem::getInstance()->setGlobalMusicVolume(Config::getFloat("musicvol"));

		if ((int) m_enableSound->isChecked() != Config::getInt("sound"))
		{
			m_enableSound->check(!m_enableSound->isChecked());
			if (Config::getInt("sound"))
				AudioSystem::getInstance()->activate();
			else
				AudioSystem::getInstance()->deactivate();
		}

		doCommand();
	}

}

void ConfigDialog::show()
{
	m_oldFullscreen = Config::getInt("fullscreen");
	m_oldShowFps = Config::getInt("showfps");
	m_oldResolution = VideoSystem::getInstance()->getResolution();
	m_oldRenderer = (Config::getString("renderer") == "opengl");

	Widget::show();
}
