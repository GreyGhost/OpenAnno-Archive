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

#include "imagebutton.h"
#include "widgetmanager.h"

ImageButton::ImageButton(int x, int y, const std::string& caption, const std::string& image, const std::string& imageHighlighted, int commandId) : CommandWidget(x, y, commandId), m_caption(caption), m_isHighlighted(false)
{
	m_image = VideoSystem::getInstance()->createSurface(image);
	m_imageHighlighted = VideoSystem::getInstance()->createSurface(imageHighlighted);

	m_width = VideoSystem::getInstance()->getDefaultFont()->getTextWidth(caption, Font::SIZE_SMALL);
	m_height = VideoSystem::getInstance()->getDefaultFont()->getTextHeight(caption, Font::SIZE_SMALL);
	if(m_image.isNotNull())
	{
		m_textOffsetY = (m_image->getHeight() - m_height) / 2;
		m_width += m_image->getWidth() + 5;
		if(m_image->getHeight() > m_height)
		{
			m_height = m_image->getHeight();
		}
	}
	else
	{
		m_textOffsetY = 0;
	}

	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<ImageButton, EventMouseOver>(this, &ImageButton::onMouseOver));
	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<ImageButton, EventMouseOut>(this, &ImageButton::onMouseOut));
	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<ImageButton, EventMouseClick>(this, &ImageButton::onMouseClick));
}

ImageButton::~ImageButton()
{
}

void ImageButton::draw(const VideoSystem* const screen)
{
	if(!m_isHighlighted)
	{
		if(m_image.isNotNull())
		{
			screen->drawSurface(m_image, m_absX, m_absY);
			screen->drawText(m_caption, m_absX + m_image->getWidth() + 5, m_absY + m_textOffsetY, Color::GRAY30, Font::SIZE_SMALL);
		}
		else
		{
			screen->drawText(m_caption, m_absX, m_absY, Color::GRAY30, Font::SIZE_SMALL);
		}
	}
	else
	{
		if(m_imageHighlighted.isNotNull())
		{
			screen->drawSurface(m_imageHighlighted, m_absX, m_absY);
			screen->drawText(m_caption, m_absX + m_imageHighlighted->getWidth() + 5, m_absY + m_textOffsetY, Color::BLACK, Font::SIZE_SMALL);
		}
		else
		{
			screen->drawText(m_caption, m_absX, m_absY, Color::BLACK, Font::SIZE_SMALL);
		}
	}
	Widget::draw(screen);
}

void ImageButton::onMouseOut(EventMouseOut* event)
{
	m_isHighlighted = false;
}

void ImageButton::onMouseOver(EventMouseOver* event)
{
	m_isHighlighted = true;
}

void ImageButton::onMouseClick(EventMouseClick* event)
{
	if(event->m_button == EventMouseClick::BTN_LEFT && !event->m_isButtonDown && hasFocus())
	{
		doCommand();
	}
}
