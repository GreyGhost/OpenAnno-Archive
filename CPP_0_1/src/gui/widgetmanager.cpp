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

#include <assert.h>
#include "widgetmanager.h"
#include "widget.h"
#include "events/eventmousemove.h"
#include "events/eventmouseclick.h"
#include "events/eventmouseover.h"
#include "events/eventmouseout.h"
#include "events/eventkey.h"
#include "events/eventtick.h"

#define MIN(a, b) ((a)<(b) ? (a) : (b))

WidgetManager::WidgetManager() : m_mouseCaper(NULL), m_focusedWidget(NULL), m_hoveredWidget(NULL), m_hoveredTime(0.0f), m_mouseX(0), m_mouseY(0), /*m_modalWidget(NULL),*/ m_tooltipX(0), m_tooltipY(0), m_isTooltipPosSet(false), m_showCursor(true)
{
	m_defaultCursor = VideoSystem::getInstance()->createSurface("images/misc/cursor.png");
}

WidgetManager::~WidgetManager()
{
	for(EventListeners::iterator elIt = m_eventListeners.begin(); elIt != m_eventListeners.end(); ++elIt)
	{
		ListenerDataList* data = &elIt->second;
		for(ListenerDataList::iterator dataIt = data->begin(); dataIt != data->end(); ++dataIt)
		{
			delete (*dataIt);
			(*dataIt) = NULL;
		}
		data->clear();
	}
	m_eventListeners.clear();

	m_mouseCaper = NULL;
	m_focusedWidget = NULL;
	m_hoveredWidget = NULL;
}

WidgetManager* WidgetManager::getInstance()
{
	static WidgetManager inst;
	return &inst;
}

void WidgetManager::sendEvent(Event* event, Widget* eventSender)
{
	assert(event);
	event->setSender(eventSender);
	EventListeners::iterator ELit = m_eventListeners.find(event->getType());
	if(ELit != m_eventListeners.end())
	{
		ListenerDataList* data = &ELit->second;

		for(ListenerDataList::iterator dataIt = data->begin(); dataIt != data->end(); ++dataIt)
		{
			if((!(*dataIt)->remove) && (!(*dataIt)->m_eventSender || (*dataIt)->m_eventSender == eventSender)
			 && ((*dataIt)->m_alwaysSend || (event->shouldBeSendTo((*dataIt)->m_eventListener->m_listener)))
			 /*&& (!m_modalWidget || m_modalWidget == (*dataIt)->m_eventListener->m_listener || (*dataIt)->m_eventListener->m_listener->isChildOf(m_modalWidget))))*/
			)
			{
				(*dataIt)->m_eventListener->handleEvent(event);
			}
		}
	}
	delete event;
}

void WidgetManager::addEventListener(EventListener* eventListener, Widget* eventSender, bool alwaysSend)
{
	m_eventListeners[eventListener->m_eventType].push_back(new ListenerData(eventListener, eventSender, alwaysSend));
}

void WidgetManager::remEventListener(EventListener* eventListener, Widget* eventSender)
{
	EventListeners::iterator elIt = m_eventListeners.find(eventListener->m_eventType);
	if(elIt != m_eventListeners.end())
	{
		ListenerDataList* data = &elIt->second;

		for(ListenerDataList::iterator dataIt = data->begin(); dataIt != data->end(); ++dataIt)
		{
			if((*dataIt)->m_eventListener->m_listener == eventListener->m_listener && (!eventSender || (*dataIt)->m_eventSender == eventSender))
			{
				(*dataIt)->remove = true;
			}
		}
	}
}

void WidgetManager::remAllEventListeners(Widget* listener)
{
	for(EventListeners::iterator elIt = m_eventListeners.begin(); elIt != m_eventListeners.end(); ++elIt)
	{
		ListenerDataList* data = &elIt->second;
		for(ListenerDataList::iterator dataIt = data->begin(); dataIt != data->end(); ++dataIt)
		{
			if((*dataIt)->m_eventListener->m_listener == listener)
			{
				(*dataIt)->remove = true;
			}
		}
	}
}

void WidgetManager::updateWidgetUnderMouse()
{
	if(m_mouseCaper != NULL)
	{
		return;
	}
	Widget* widgetToBeHovered = NULL;

	for(Widget::Widgets::iterator it = m_widgets.begin(); it != m_widgets.end(); ++it)
	{
		if((*it)->isPointInWidget(m_mouseX, m_mouseY) && (*it)->isEnabled() && (*it)->isVisible())
		{
			if(!widgetToBeHovered)
			{
				widgetToBeHovered = (*it);
			}
			else if(widgetToBeHovered->getLevel() <= (*it)->getLevel())
			{
				widgetToBeHovered = (*it);
			}
		}
	}
/*	if(m_modalWidget && !widgetToBeHovered->isChildOf(m_modalWidget))
	{
		widgetToBeHovered = m_modalWidget;
	}*/
	if(widgetToBeHovered != m_hoveredWidget) {
		sendEvent(new EventMouseOut(m_hoveredWidget), NULL);
		sendEvent(new EventMouseOver(widgetToBeHovered), NULL);
		m_hoveredWidget = widgetToBeHovered;

		if(m_hoveredWidget)
		{
			m_widgetCursor = m_hoveredWidget->getCursor();
			m_isTooltipPosSet = false;
			m_hoveredTime = 0.0f;
		}
	}
}
void WidgetManager::handleSDLInput(const SDL_Event& event)
{
	switch(event.type)
	{
		case SDL_KEYDOWN:
		{
			sendEvent(new EventKey(event.key.keysym.sym, true, event.key.keysym.mod, ((event.key.keysym.unicode & 0xFF80) == 0 ? event.key.keysym.unicode & 0x7F : 0)), NULL);
		}
		break;
		case SDL_KEYUP:
		{
			sendEvent(new EventKey(event.key.keysym.sym, false, event.key.keysym.mod, ((event.key.keysym.unicode & 0xFF80) == 0 ? event.key.keysym.unicode & 0x7F : 0)), NULL);
		}
		break;
		case SDL_MOUSEMOTION:
		{
			m_mouseX = event.button.x;
			m_mouseY = event.button.y;

			updateWidgetUnderMouse();
			sendEvent(new EventMouseMove(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel), NULL);
		}
		break;
		case SDL_MOUSEBUTTONDOWN:
		{
			if(m_hoveredWidget)
			{
				setFocus(m_hoveredWidget);
			}

			switch (event.button.button) {
				case SDL_BUTTON_LEFT:
					sendEvent(new EventMouseClick(EventMouseClick::BTN_LEFT, event.button.x, event.button.y, true), NULL);
					break;
				case SDL_BUTTON_MIDDLE:
					sendEvent(new EventMouseClick(EventMouseClick::BTN_MIDDLE, event.button.x, event.button.y, true), NULL);
					break;
				case SDL_BUTTON_RIGHT:
					sendEvent(new EventMouseClick(EventMouseClick::BTN_RIGHT, event.button.x, event.button.y, true), NULL);
					break;
				case SDL_BUTTON_WHEELUP:
					sendEvent(new EventMouseClick(EventMouseClick::BTN_WHEELUP, event.button.x, event.button.y, true), NULL);
					break;
				case SDL_BUTTON_WHEELDOWN:
					sendEvent(new EventMouseClick(EventMouseClick::BTN_WHEELDOWN, event.button.x, event.button.y, true), NULL);
			}
		}
		break;
		case SDL_MOUSEBUTTONUP:
		{
			switch (event.button.button) {
				case 1:
					sendEvent(new EventMouseClick(EventMouseClick::BTN_LEFT, event.button.x, event.button.y, false), NULL);
					break;
				case 2:
					sendEvent(new EventMouseClick(EventMouseClick::BTN_MIDDLE, event.button.x, event.button.y, false), NULL);
					break;
				case 3:
					sendEvent(new EventMouseClick(EventMouseClick::BTN_RIGHT, event.button.x, event.button.y, false), NULL);
			}
		}
		break;
		default:
		{
			assert(0);
		}
	}
}

void WidgetManager::updateFrametime(float delayTime)
{
	// Remove all Eventlisteners that were marked to be removed
	for(EventListeners::iterator elIt = m_eventListeners.begin(); elIt != m_eventListeners.end(); ++elIt)
	{
		ListenerDataList* data = &elIt->second;
		for(ListenerDataList::iterator dataIt = data->begin(); dataIt != data->end();)
		{
			if((*dataIt)->remove)
			{
				delete (*dataIt);
				(*dataIt) = NULL;
				dataIt = data->erase(dataIt);
				if(dataIt == data->end())
				{
					break;
				}
			}
			else
			{
				++dataIt;
			}
		}
	}

	sendEvent(new EventTick(delayTime), NULL);

	m_hoveredTime += delayTime;
}

WidgetManager::ListenerData::ListenerData(EventListener* eventListener, Widget* eventSender, bool alwaysSend) : m_eventListener(eventListener), m_eventSender(eventSender), m_alwaysSend(alwaysSend), remove(false)
{
}

WidgetManager::ListenerData::~ListenerData()
{
	delete m_eventListener;
	m_eventListener = NULL;
}

bool WidgetManager::captureMouse(Widget* widget)
{
	if(m_mouseCaper == NULL)
	{
		m_mouseCaper = widget;
		return true;
	}
	return false;
}

void WidgetManager::releaseMouseCapture(Widget* widget)
{
	assert(widget == m_mouseCaper);
	m_mouseCaper = NULL;
	updateWidgetUnderMouse();
}

bool WidgetManager::hasMouseCaptured(Widget* widget)
{
	return m_mouseCaper == widget;
}

void WidgetManager::setFocus(Widget* widget)
{
	m_focusedWidget = widget;
}

bool WidgetManager::hasFocus(Widget* widget)
{
	return m_focusedWidget == widget;
}

void WidgetManager::registerWidget(Widget* widget)
{
	m_widgets.push_back(widget);
}

void WidgetManager::unregisterWidget(Widget* widget)
{
	m_widgets.remove(widget);
	remAllEventListeners(widget);
/*	if(widget == m_modalWidget)
	{
		m_modalWidget = NULL;
	}*/

	updateWidgetUnderMouse();
}

void WidgetManager::draw()
{
	for(Widget::Widgets::iterator it = m_widgets.begin(); it != m_widgets.end(); ++it)
	{
		if((*it)->getParent() == NULL && (*it)->isVisible())// && (*it) != m_modalWidget)
		{
			(*it)->draw(VideoSystem::getInstance());
		}
	}
/*	if(m_modalWidget)
	{
		m_modalWidget->draw(VideoSystem::getInstance());
	}*/

	if(m_hoveredWidget != NULL && m_hoveredWidget->getTooltipText() != "" && m_hoveredTime >= 1.0f)
	{
		std::string tooltipText = m_hoveredWidget->getTooltipText();
		if(tooltipText != "")
		{
			int tooltipWidth = VideoSystem::getInstance()->getDefaultFont()->getTextWidth(tooltipText, Font::SIZE_SMALL) + 4;
			int tooltipHeight = VideoSystem::getInstance()->getDefaultFont()->getTextHeight(tooltipText, Font::SIZE_SMALL) + 4;

			if(!m_isTooltipPosSet)
			{
				m_tooltipX = m_mouseX + m_hoveredWidget->getWidth()/2 - tooltipWidth;
				m_tooltipY = m_hoveredWidget->getAbsY() - tooltipHeight;

				m_isTooltipPosSet = true;
			}
			VideoSystem::getInstance()->drawFilledRect(m_tooltipX, m_tooltipY, tooltipWidth, tooltipHeight, Color(1.0f,1.0f,220.0f/255.0f, MIN(m_hoveredTime - 1.0f, 0.5f) * 2.0f));
			VideoSystem::getInstance()->drawRect(m_tooltipX, m_tooltipY, tooltipWidth, tooltipHeight, Color(0.0f,0.0f,0.0f, MIN(m_hoveredTime - 1.0f, 0.5f) * 2.0f));
			VideoSystem::getInstance()->drawText(tooltipText, m_tooltipX + 2, m_tooltipY + 2, Color(0.0f, 0.0f, 0.0f, MIN(m_hoveredTime - 1.0f, 0.5f) * 2.0f), Font::SIZE_SMALL);
		}
	}
	if(m_showCursor)
	{
		if(m_widgetCursor.isNotNull())
		{
			VideoSystem::getInstance()->drawSurface(m_widgetCursor, m_mouseX, m_mouseY);
		}
		else if(m_defaultCursor.isNotNull())
		{
			VideoSystem::getInstance()->drawSurface(m_defaultCursor, m_mouseX, m_mouseY);
		}
	}
}

void WidgetManager::showCursor()
{
	m_showCursor = true;
}

void WidgetManager::hideCursor()
{
	m_showCursor = false;
}

bool WidgetManager::isCursorVisible()
{
	return m_showCursor;
}

/*bool WidgetManager::makeWidgetModal(Widget* widget)
{
	if(widget->getParent() != NULL || m_modalWidget != NULL)
	{
		return false;
	}
	m_modalWidget = widget;
	updateWidgetUnderMouse();
	return true;
}*/
