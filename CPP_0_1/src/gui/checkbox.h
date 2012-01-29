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

#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <string>
#include "video/videosystem.h"
#include "video/surface.h"
#include "gui/events/eventmouseclick.h"
#include "commandwidget.h"

class Checkbox : public CommandWidget
{
public:
	Checkbox(int x, int y, const std::string& caption, int commandId = -1);
	virtual ~Checkbox();
	void draw(const VideoSystem* const screen);
	void mouseClick(EventMouseClick* event);
	bool isChecked() const { return m_checked; };
	void check(bool checked = true) { m_checked = checked; };

protected:
	std::string m_caption;
	SurfacePtr m_box;
	SurfacePtr m_checkedBox;
	bool m_checked;
};


#endif
