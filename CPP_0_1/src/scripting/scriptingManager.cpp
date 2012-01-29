/***************************************************************************
 *   Copyright (C) 2006 by Stefan Geiger                                   *
 *   stegei@real-world.ch                                                  *
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

#include "scriptingManager.h"
#include "class.h"
#include "luaScriptSystem.h"

ScriptingManager ScriptingManager::m_instance;

ScriptingManager* ScriptingManager::getInstance()
{
    return &m_instance;
}

void ScriptingManager::registerClass(Class* pClass)
{
    m_classes.insert(std::make_pair(pClass->getName(), pClass));
}

Class* ScriptingManager::getClass(const char* pClassname)
{
    std::map<std::string, Class*>::iterator it = m_classes.find(pClassname);
    if (it != m_classes.end()) {
        return it->second;
    }
    return NULL;
}

LuaScriptSystem* ScriptingManager::getScriptSystem()
{
    return m_pScriptSystem;
}

ScriptingManager::ScriptingManager()
{
    m_pScriptSystem = new LuaScriptSystem();
}

ScriptingManager::~ScriptingManager()
{
    delete m_pScriptSystem;
    m_pScriptSystem = NULL;

    std::map<std::string, Class*>::iterator it;
    for (it = m_classes.begin(); it != m_classes.end(); ++it) {
        delete it->second;
    }
    m_classes.clear();
}
