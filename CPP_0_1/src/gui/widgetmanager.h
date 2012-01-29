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

#ifndef WIDGETMANAGER_H
#define WIDGETMANAGER_H

#include <map>
#include <list>
#include <assert.h>
#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL/SDL.h>
#endif
#include "events/event.h"
#include "widget.h"

/** \addtogroup eventsandwidgets
 *
 * \{
 */

class EventListener
{
friend class WidgetManager;
public:
	virtual ~EventListener() {}
	EventListener(EventType eventType, Widget* listener) : m_eventType(eventType), m_listener(listener) { };
	virtual void handleEvent(Event* event) = 0;
protected:
	const EventType m_eventType;
	Widget* m_listener;
};

template <class ListenerClass, class EventClass> class SpecificEventListener : public EventListener
{
public:
	void handleEvent(Event* event) { assert(event->getType() == EventClass::TYPE); (static_cast<ListenerClass*>(m_listener)->*m_eventHandler)(static_cast<EventClass*>(event)); };
	SpecificEventListener(ListenerClass* listener, void(ListenerClass::*eventHandler)(EventClass*)) :  EventListener(EventClass::TYPE, listener), m_eventHandler(eventHandler) {  };
private:
	void (ListenerClass::*m_eventHandler)(EventClass*);
};

class WidgetManager
{
public:
	static WidgetManager* getInstance();
	void addEventListener(EventListener* eventListener, Widget* eventSender = NULL, bool alwaysSend = false);
	void remEventListener(EventListener* eventListener, Widget* eventSender = NULL);
	void remAllEventListeners(Widget* listener);
	void sendEvent(Event* event, Widget* eventSender);

	void registerWidget(Widget* widget);
	void unregisterWidget(Widget* widget);

	void handleSDLInput(const SDL_Event& event);
	void updateFrametime(float delayTime);

	bool captureMouse(Widget* widget);
	void releaseMouseCapture(Widget* widget);
	bool hasMouseCaptured(Widget* widget);
	void setFocus(Widget* widget);
	bool hasFocus(Widget* widget);

	void updateWidgetUnderMouse();

//	bool makeWidgetModal(Widget* widget);

	void draw();

	void showCursor();
	void hideCursor();
	bool isCursorVisible();

	/* This has to be moved to an inputmanager*/
	int getMouseX() { return m_mouseX; };
	int getMouseY() { return m_mouseY; };

	~WidgetManager();
private:
	class ListenerData
	{
	public:
		ListenerData(EventListener* eventListener, Widget* eventSender, bool alwaysSend);
		~ListenerData();

		EventListener* m_eventListener;
		Widget* const m_eventSender;
		bool m_alwaysSend;
		// This is set true if the ListerData should be removed from the Widgetmanagers EventListeners
		// We need to do this because if we erase a field from the list while iterateing through the list we will cause problems
		// This hapens if a Widget decides on receiving a event to remove the event listener
		bool remove;
	};

	typedef std::list<ListenerData*> ListenerDataList;

	typedef std::map<EventType, ListenerDataList > EventListeners;

	WidgetManager();
	EventListeners m_eventListeners;

	Widget* m_mouseCaper;
	Widget* m_focusedWidget;
	Widget* m_hoveredWidget;
	float m_hoveredTime;
	Widget::Widgets m_widgets;
	int m_mouseX, m_mouseY;

//	Widget* m_modalWidget;

	int m_tooltipX, m_tooltipY;
	bool m_isTooltipPosSet;

	SurfacePtr m_defaultCursor;
	SurfacePtr m_widgetCursor;
	bool m_showCursor;
};

/** \} */

#endif
