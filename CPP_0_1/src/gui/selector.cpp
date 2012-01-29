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

#include "selector.h"
#include "widgetmanager.h"

Selector::Selector(int x, int y, Type type, int commandId) : CommandWidget(x, y, commandId), m_type(type), m_selectedItem(-1)
{
	m_width = 175;

	if(m_type == VERTICAL_SELECTOR)
	{
		m_arrows[0] = VideoSystem::getInstance()->createSurface("images/hud/up.png");
		m_arrows[1] = VideoSystem::getInstance()->createSurface("images/hud/down.png");
		if(m_arrows[0].isNotNull() && m_arrows[1].isNotNull())
		{
			m_height = m_arrows[0]->getHeight() + m_arrows[1]->getHeight();
		}
		else
		{
			m_height = VideoSystem::getInstance()->getDefaultFont()->getTextHeight(" ", Font::SIZE_SMALL);
		}
	}
	else
	{
		m_arrows[0] = VideoSystem::getInstance()->createSurface("images/hud/left.png");
		m_arrows[1] = VideoSystem::getInstance()->createSurface("images/hud/right.png");
		if(m_arrows[0].isNotNull())
		{
			m_height = m_arrows[0]->getHeight();
		}
		else if(m_arrows[1].isNotNull())
		{
			m_height = m_arrows[1]->getHeight();
		}
		else
		{
			m_height = VideoSystem::getInstance()->getDefaultFont()->getTextHeight(" ", Font::SIZE_SMALL);
		}
	}

	WidgetManager::getInstance()->addEventListener(new SpecificEventListener<Selector, EventMouseClick>(this, &Selector::mouseClick));
}

Selector::~Selector()
{
}

void Selector::addItem(const std::string& caption, const std::string& name )
{
	if(m_selectedItem == -1)
	{
		m_selectedItem = 0;
	}
	m_itemsTitle.push_back(caption);
	if(name == "")
	{
		m_itemsName.push_back(caption);
	}
	else
	{
		m_itemsName.push_back(name);
	}
}

std::string Selector::getSelectedItemTitle() const
{
	return m_itemsTitle[m_selectedItem];
}

std::string Selector::getSelectedItemName() const
{
	return m_itemsName[m_selectedItem];
}

int Selector::getSelectedItemIndex() const
{
	return m_selectedItem;
}

void Selector::selectItem(int index)
{
	if(index < 0)
	{
		m_selectedItem = 0;
	}
	else if(index > static_cast<int>(m_itemsTitle.size()) - 1)
	{
		m_selectedItem = static_cast<int>(m_itemsTitle.size()) - 1;
	}
	else
	{
		m_selectedItem = index;
	}
}

void Selector::mouseClick(EventMouseClick* event)
{
	if(m_arrows[0].isNull() || m_arrows[1].isNull())
		return;

	if(hasFocus() && !event->m_isButtonDown)
	{
		int oldSelectedItem = m_selectedItem;
		if(m_type == VERTICAL_SELECTOR)
		{
			if((event->m_absX >= m_absX) && (event->m_absX < (m_absX + m_arrows[0]->getWidth())))
			{
				if((event->m_absY >= m_absY) && (event->m_absY < (m_absY + m_arrows[0]->getHeight())))
				{
					--m_selectedItem;
				}
				else
				{
					++m_selectedItem;
				}
			}
		}
		else
		{
			if((event->m_absY >= m_absY) && (event->m_absY < (m_absY + m_height)))
			{
				if((event->m_absX >= m_absX) && (event->m_absX < (m_absX + m_arrows[0]->getWidth())))
				{
					--m_selectedItem;
				}
				else if((event->m_absX >= (m_absX + m_width - m_arrows[1]->getWidth())) && (event->m_absX < (m_absX + m_width)))
				{
					++m_selectedItem;
				}
			}
		}

		if(m_selectedItem >= static_cast<int>(m_itemsTitle.size()))
		{
			m_selectedItem = 0;//static_cast<int>(m_itemsTitle.size()) - 1;
		}
		else if(m_selectedItem < 0)
		{
			m_selectedItem = static_cast<int>(m_itemsTitle.size()) - 1;//0;
		}
		if(oldSelectedItem != m_selectedItem)
		{
			doCommand();
		}
	}
}

void Selector::draw(const VideoSystem* const screen)
{
	if(m_type == VERTICAL_SELECTOR)
	{
		if(m_arrows[0].isNotNull())
		{
			screen->drawSurface(m_arrows[0], m_absX, m_absY/*, (m_selectedItem <= 0 ? Color::GRAY : Color::WHITE)*/);
		}
		if(m_arrows[1].isNotNull())
		{
			screen->drawSurface(m_arrows[1], m_absX, m_absY + m_arrows[0]->getHeight()/*, (m_selectedItem == static_cast<int>(m_items.size()) - 1 ? Color::GRAY : Color::WHITE)*/);
		}
		if(m_itemsTitle.size())
		{
			if(m_arrows[0].isNotNull())
			{
				int textOffsetY = (m_height - VideoSystem::getInstance()->getDefaultFont()->getTextHeight(m_itemsTitle[m_selectedItem], Font::SIZE_SMALL)) / 2;
				screen->drawText(m_itemsTitle[m_selectedItem], m_absX + m_arrows[0]->getWidth(), m_absY + textOffsetY, Color::BLACK, Font::SIZE_SMALL);
			}
			else
			{
				screen->drawText(m_itemsTitle[m_selectedItem], m_absX, m_absY, Color::BLACK, Font::SIZE_SMALL);
			}
		}
	}
	else
	{
		if(m_arrows[0].isNotNull())
		{
			screen->drawSurface(m_arrows[0], m_absX, m_absY/*, (m_selectedItem <= 0 ? Color::GRAY : Color::WHITE)*/);
		}
		if(m_arrows[1].isNotNull())
		{
			screen->drawSurface(m_arrows[1], m_absX + m_width - m_arrows[1]->getWidth(), m_absY/*, (m_selectedItem == static_cast<int>(m_items.size()) - 1 ? Color::GRAY : Color::WHITE)*/);
		}
		if(m_itemsTitle.size())
		{
			if(m_arrows[0].isNotNull())
			{
				int textOffsetX = (m_width - m_arrows[0]->getWidth() - m_arrows[1]->getWidth() - VideoSystem::getInstance()->getDefaultFont()->getTextWidth(m_itemsTitle[m_selectedItem], Font::SIZE_SMALL)) / 2;
				screen->drawText(m_itemsTitle[m_selectedItem], m_absX + m_arrows[0]->getWidth() + textOffsetX, m_absY, Color::BLACK, Font::SIZE_SMALL);
			}
			else
			{
				screen->drawText(m_itemsTitle[m_selectedItem], m_absX, m_absY, Color::BLACK, Font::SIZE_SMALL);
			}
		}
	}

	Widget::draw(screen);
}

bool Selector::empty()
{
	assert(m_itemsTitle.size() == m_itemsName.size());
	return m_itemsTitle.empty();
}
