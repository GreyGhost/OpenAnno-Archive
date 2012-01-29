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

#ifndef WIDGET_H
#define WIDGET_H

#include <list>
#include <map>
#include "video/videosystem.h"
#include "events/event.h"


class Widget
{
public:
	Widget(int x, int y);
	virtual ~Widget();
	virtual void enable();
	virtual void disable();
	virtual bool isEnabled();
	virtual void show();
	virtual void hide();
	virtual bool isVisible();
	virtual void draw(const VideoSystem* const screen);
	virtual void addChild(Widget* child);
	virtual void setRelPos(int x, int y);
	virtual int getRelX() const { return m_relX; };
	virtual int getRelY() const { return m_relY; };
	virtual int getAbsX() const { return m_absX; };
	virtual int getAbsY() const { return m_absY; };
	virtual int getWidth() const { return m_width; };
	virtual int getHeight() const { return m_height; };
	virtual void captureMouse();
	virtual void releaseMouseCapture();
	virtual bool hasMouseCaptured();
	virtual void setFocus();
	virtual bool hasFocus();
	/**
	 * \brief Set the widget´s tooltip
	 * \param tooltip The new tooltip
	 */
	virtual void setTooltipText(std::string tooltip);
	/**
	 * \brief Get the widget´s tooltip
	 * \return The widget´s current tooltip
	 */
	virtual std::string getTooltipText(void) const;
	virtual Widget* getParent() { return m_parent; };
	virtual SurfacePtr getCursor() const;
	virtual unsigned int getLevel();
	virtual bool isChildOf(Widget* widget) { if(widget == m_parent) { return true; } if(!m_parent) return false; return m_parent->isChildOf(widget); };

	virtual bool isPointInWidget(int x, int y);

	typedef std::list<Widget*> Widgets;
protected:
	virtual void sendEvent(Event* event);
	virtual void setParent(Widget* parent);
	virtual void removeChild(Widget* child);
	virtual void recalculateAbsPos();
	virtual void recalculateLevel();

	int m_relX, m_relY;
	int m_absX, m_absY;
	int m_width, m_height;
	Widget* m_parent;
	Widgets m_children;
	bool m_isEnabled;
	bool m_isVisible;
	/**
	 * \brief The widget´s tooltip
	 */
	std::string m_strTooltip;

	unsigned int m_level;
	SurfacePtr m_cursor;
};


#endif
