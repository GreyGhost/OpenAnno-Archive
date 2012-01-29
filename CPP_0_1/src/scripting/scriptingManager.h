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

#ifndef SCRIPTINGMANAGER_H
#define SCRIPTINGMANAGER_H

#include <map>
#include <string>

class LuaScriptSystem;
class Class;

class ScriptingManager
{
public:
    static ScriptingManager* getInstance();

    void registerClass(Class* pClass);
    Class* getClass(const char* pClassname);

    LuaScriptSystem* getScriptSystem();
private:
    ScriptingManager();
    ~ScriptingManager();

    static ScriptingManager m_instance;

    std::map<std::string, Class*> m_classes;
    LuaScriptSystem* m_pScriptSystem;
};

#endif // SCRIPTINGMANAGER_H
