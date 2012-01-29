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

#include "slider.h"
#include "widgetmanager.h"


Slider::Slider(int x, int y, int min, int max, int commandId) : CommandWidget(x, y, commandId), m_min(min), m_max(max), m_pos(min), m_dragButtonPos(0), m_doDrag(false), m_dragButtonOffsetY(0)
{
	m_dragButton = VideoSystem::getInstance()->createSurface("images/hud/bar.png");

	if(m_dragButton.isNotNull())
	{
		m_height = m_dragButton->getHeight();
	}
	else
	{
		m_height = 20;
	}
    m_width = 200;

	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<Slider, EventMouseClick>(this, &Slider::mouseClick));
	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<Slider, EventMouseMove>(this, &Slider::mouseMove));
}

Slider::~Slider()
{
}

std::string Slider::getTooltipText(void) const
{
	std::stringstream ret;
	ret <<  "Aktueller Wert: " <<  m_pos <<  "/" << m_max;
	return ret.str();
}

void Slider::draw(const VideoSystem* const screen)
{
	screen->drawFilledRect(m_absX, m_absY+(m_height-6)/2, m_width, 6, Color(0.7f, 0.7f, 0.7f, 0.5f));
	screen->drawRect(m_absX, m_absY+(m_height-6)/2, m_width, 6, Color::BLACK);
	if(m_dragButton.isNotNull())
	{
		screen->drawSurface(m_dragButton, m_absX+m_dragButtonPos, m_absY);
	}
	else
	{
		screen->drawRect(m_absX+m_dragButtonPos, m_absY, m_height, 10, Color::BLACK);
	}

	Widget::draw(screen);
}

void Slider::mouseClick(EventMouseClick* event)
{
	if(event->m_button == EventMouseClick::BTN_LEFT)
	{
		if(event->getRelX(this) >= m_dragButtonPos && event->getRelY(this) >= 0 && event->getRelX(this) < m_dragButtonPos+(m_dragButton.isNotNull() ? m_dragButton->getWidth() : 20) && event->getRelY(this) < m_height)
		{
			if(event->m_isButtonDown)
			{
				captureMouse();
				m_doDrag = true;
				m_dragButtonOffsetY = m_dragButtonPos - event->getRelX(this);
			}
		} else if (event->m_isButtonDown) {
			if (abs(event->getRelX(this) - m_dragButtonPos-(m_dragButton->getWidth()/2)) < 20) {
				m_dragButtonPos = event->getRelX(this)-(m_dragButton->getWidth()/2);
			} else if (event->getRelX(this) < m_dragButtonPos) {
				m_dragButtonPos -= m_width / 10; // move by 10%
			} else if (event->getRelX(this) > m_dragButtonPos) {
				m_dragButtonPos += m_width / 10;
			}

			if(m_dragButtonPos < 0)
			{
				m_dragButtonPos = 0;
			}
			else if(m_dragButtonPos + (m_dragButton.isNotNull() ? m_dragButton->getWidth() : 20) > m_width)
			{
				m_dragButtonPos = m_width - (m_dragButton.isNotNull() ? m_dragButton->getWidth() : 20);
			}

			int oldPos = m_pos;
			m_pos = m_min + ((m_max - m_min) * m_dragButtonPos / (m_width-(m_dragButton.isNotNull() ? m_dragButton->getWidth() : 20)));
			if(m_pos != oldPos)
			{
				doCommand();
			}
		}
		if(m_doDrag && !event->m_isButtonDown)
		{
			m_doDrag = false;
		}

		if(!event->m_isButtonDown && hasMouseCaptured())
		{
			releaseMouseCapture();
		}
	}
}
/*  m_min = minimal of the scale
    m_pos = actuale value (scale value)
    m_max = maximal of the scale
    m_width = width of Slide

    */
void Slider::mouseMove(EventMouseMove* event)
{
	if(m_doDrag)
	{
		m_dragButtonPos = event->getRelX(this) + m_dragButtonOffsetY;
		if(m_dragButtonPos < 0)
		{
			m_dragButtonPos = 0;
		}
		else if(m_dragButtonPos + (m_dragButton.isNotNull() ? m_dragButton->getWidth() : 20) > m_width)
		{
			m_dragButtonPos = m_width - (m_dragButton.isNotNull() ? m_dragButton->getWidth() : 20);
		}

		int oldPos = m_pos;
		m_pos = m_min + ((m_max - m_min) * m_dragButtonPos / (m_width-(m_dragButton.isNotNull() ? m_dragButton->getWidth() : 20)));
		if(m_pos != oldPos)
		{
			doCommand();
		}
	}
}

bool Slider::isPointInWidget(int x, int y)
{
	if((x < m_absX) || (x >= (m_absX + m_width)))
	{
		return false;
	}

	if((x >= (m_absX + m_dragButtonPos)) && (x < (m_absX + m_dragButtonPos + (m_dragButton.isNotNull() ? m_dragButton->getWidth() : 20))) && (y >= m_absY) && (y < (m_absY + (m_dragButton.isNotNull() ? m_dragButton->getWidth() : 20))))
	{
		return true;
	}

	if((y >= (m_absY + (m_height - 6) / 2)) && (y < (m_absY + (m_height - 6) / 2) + 6))
	{
		return true;
	}

	return false;
}

void Slider::setPos(int pos)
{
    m_pos = pos;
	if(pos < m_min)
	{
		m_pos = m_min;
	}
	else if(pos > m_max)
	{
		m_pos = m_max;
	}
	else
	{
		m_pos = pos;
	}
	m_dragButtonPos = ((m_pos - m_min) * (m_width-(m_dragButton.isNotNull() ? m_dragButton->getWidth() : 20))) /  (m_max - m_min);
}
