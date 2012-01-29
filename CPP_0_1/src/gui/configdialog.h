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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <string>
#include "dialog.h"
#include "editbox.h"
#include "slider.h"
#include "checkbox.h"
#include "selector.h"
#include "imagebutton.h"
#include "gui/events/eventcommand.h"

class ConfigDialog : public Dialog
{
public:
	ConfigDialog(int x, int y, int width, int height, const std::string& background);
	~ConfigDialog();
	void show();
private:
	void onCommand(EventCommand* event);
	Editbox* m_playerName;
	Checkbox* m_showFPS;
	Checkbox* m_fullscreen;
	Checkbox* m_enableSound;
	Slider* m_scrollSpeed;
	Slider* m_musicVolume;
	Slider* m_soundVolume;
	Slider* m_gamma;
	Selector* m_videoModes;
	Selector* m_renderer;
	VideoModes m_videoModeList;
	ImageButton* m_Ok;
	ImageButton* m_Cancel;

	int m_oldShowFps;
	int m_oldFullscreen;
	int m_oldRenderer;
	VideoMode m_oldResolution;
};


#endif
