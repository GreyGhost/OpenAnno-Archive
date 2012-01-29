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

#ifndef LUASCRIPTSYSTEM_H
#define LUASCRIPTSYSTEM_H

#include <vector>


extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

class MethodArguments;

class LuaScriptSystem
{
public:
    LuaScriptSystem();
    ~LuaScriptSystem();

    int doString(const char* pStr);
    int doBuffer(const char* pData, unsigned int len, const char* pName);

    void addMethod(int (*pMethod), const char* pMethodName);
    int invokeMethod(void* pData, const char* pMethodName);
    bool invokeConstructor(void* pData, const char* pClassName);
    void invokeScriptMethod(const char* pMethodName, MethodArguments* pArguments, unsigned int numArgs, unsigned int numReturns);

private:
    bool invokeObjectMethod(const char* pObjectName, const char* pMethodName, MethodArguments* pArguments, int& numReturns);
    void fillMethodArguments(lua_State* pLuaState, unsigned int startArg, unsigned int numArgs, MethodArguments* pArguments);

    lua_State* m_pLuaState;
};


#endif // LUASCRIPTSYSTEM_H_
