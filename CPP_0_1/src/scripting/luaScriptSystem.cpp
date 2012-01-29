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

#include <iostream>
#include <cstring>
#include "luaScriptSystem.h"
#include "method.h"
#include "methodArguments.h"
#include "scriptingManager.h"
#include "class.h"
#include "../entityManager.h"
#include "../entity.h"

LuaScriptSystem::LuaScriptSystem()
{
    m_pLuaState = lua_open();
    luaL_openlibs(m_pLuaState);
}

LuaScriptSystem::~LuaScriptSystem()
{
    lua_close(m_pLuaState);
}

int LuaScriptSystem::doString(const char* pStr)
{
    return doBuffer(pStr, (unsigned int)strlen(pStr), pStr);
}

/**
 * Executes the lua script in the given buffer
 *
 * @param  pData data
 * @param  len length of data
 * @param  pName name
 * @return returns lua state
 */
int LuaScriptSystem::doBuffer(const char* pData, unsigned int len, const char* pName)
{
    int state = luaL_loadbuffer(m_pLuaState, pData, len, pName);

    if (state == 0) {
        state = lua_pcall(m_pLuaState, 0, LUA_MULTRET, 0);
    }

    if (state != 0) {
        lua_getglobal(m_pLuaState, "_ALERT");
        if (lua_isfunction(m_pLuaState, -1)) {
            lua_insert(m_pLuaState, -2);
            lua_call(m_pLuaState, 1, 0);
        } else {
            std::cout << "Lua::doBuffer: '" << pName << "'\n" << lua_tostring(m_pLuaState, -2) << std::endl;
            // LOG: lua_tostring(m_pLuaState, -2)
            lua_pop(m_pLuaState, 2);
        }
    }

    return state;
}

/**
 * Adds a method
 *
 * @param  pMethod function poiner
 * @param  pMethodName method name
 */
void LuaScriptSystem::addMethod(int (*pMethod), const char* pMethodName)
{
    lua_pushcfunction(m_pLuaState, (lua_CFunction)pMethod);
    lua_setglobal(m_pLuaState, pMethodName);
}

/**
 * Invoke a method
 *
 * @param  pData data poiner
 * @param  pMethodName method name
 * @return returns number of return values
 */
int LuaScriptSystem::invokeMethod(void* pData, const char* pMethodName)
{
    lua_State* pLuaState = (lua_State*)pData;

    int numParams = lua_gettop(pLuaState);
    int numReturns = 0;

    // crop number of used arguments to not overflow MethodArguments arg data
    numParams = std::min(numParams, (int)Method::MAX_NUM_ARGS+1);

    MethodArguments methodArguments;

    if (lua_isstring(pLuaState, 1)) {

        // the first parameter is the object name
        const char* pObjectName = lua_tostring(pLuaState, 1);

        fillMethodArguments(pLuaState, 1, numParams-1, &methodArguments);

        if (invokeObjectMethod(pObjectName, pMethodName, &methodArguments, numReturns)) {
            for (int i=0; i<numReturns; i++) {
                switch(methodArguments.ret[i].m_type) {
                    case Value::Float:  lua_pushnumber(pLuaState, methodArguments.ret[i].f);
                                        break;
                    case Value::String: lua_pushstring(pLuaState, methodArguments.ret[i].s);
                                        break;
                    case Value::Bool:   lua_pushboolean(pLuaState, methodArguments.ret[i].b);
                                        break;
                    default:            // nothing
                                        break;
                }
            }
        }
    }

    return numReturns;
}

/**
 * Invoke constructor
 *
 * @param  pData data poiner
 * @param  pClassName class name
 * @return returns true if sucessful otherwise false
 */
bool LuaScriptSystem::invokeConstructor(void* pData, const char* pClassName)
{
    lua_State* pLuaState = (lua_State*)pData;

    int numParams = lua_gettop(pLuaState);

    // crop number of used arguments to not overflow MethodArguments arg data
	numParams = std::min(numParams, (int)Method::MAX_NUM_ARGS);

    MethodArguments methodArguments;
    fillMethodArguments(pLuaState, 1, numParams, &methodArguments);

    Class* pClass = ScriptingManager::getInstance()->getClass(pClassName);
    if (pClass == NULL) {
        //LOG: Cant create new class instance, unknown class name
        return false;
    }

    pClass->getNewInstance(&methodArguments);

    return true;
}

/**
 * Invoke script method
 *
 * @param  pMethodName method name
 * @param  pArguments arguments
 * @param  numArgs number of argument values
 * @param  numReturns number of return values
 */
void LuaScriptSystem::invokeScriptMethod(const char* pMethodName, MethodArguments* pArguments, unsigned int numArgs, unsigned int numReturns)
{
    lua_getglobal(m_pLuaState, pMethodName);

    // check if function exists
    if (!lua_isfunction(m_pLuaState, -1)) {
        lua_pop(m_pLuaState, 1);
        return;
    }

    for (unsigned int i=0; i<numArgs; i++) {
        switch(pArguments->arg[i].m_type) {
            case Value::Float:  lua_pushnumber(m_pLuaState, pArguments->arg[i].f);
                                break;
            case Value::String: lua_pushstring(m_pLuaState, pArguments->arg[i].s);
                                break;
            case Value::Bool:   lua_pushboolean(m_pLuaState, pArguments->arg[i].b);
                                break;
            default:            // nothing
                                break;
        }
    }

    if (lua_pcall(m_pLuaState, numArgs, numReturns, 0) != 0) {
        std::cout << "Error running lua function: \n   " << lua_tostring(m_pLuaState, -1) << std::endl;
        //LOG: error running lua function
        //     lua_tostring(m_pLuaState, -1)
        lua_pop(m_pLuaState, 1);
        return;
    }

    for (unsigned int i=0; i<numReturns; i++) {
        switch (lua_type(m_pLuaState, i)) {
            case LUA_TSTRING:   pArguments->ret[i].s=lua_tostring(m_pLuaState, i);
                                pArguments->ret[i].m_type=Value::String;
                                break;
            case LUA_TBOOLEAN:  pArguments->ret[i].b=(lua_toboolean(m_pLuaState, i) == 1);
                                pArguments->ret[i].m_type=Value::Bool;
                                break;
            case LUA_TNUMBER:   pArguments->ret[i].f=(float)lua_tonumber(m_pLuaState, i);
                                pArguments->ret[i].m_type=Value::Float;
                                break;
            default:            // nothing
                                break;

        }
    }
}

/**
 * Invoke object method
 *
 * @param  pObjectName object name
 * @param  pMethodName method name
 * @param  pArguments arguments
 * @param  numReturns number of return values
 * @return returns true if sucessful otherwise false
 */
bool LuaScriptSystem::invokeObjectMethod(const char* pObjectName, const char* pMethodName, MethodArguments* pArguments, int& numReturns)
{
    Entity* pEntity = EntityManager::getInstance()->getEntityByName(pObjectName);
    if (pEntity == NULL) {
        // LOG: unknown entity name
        return false;
    }

    Class* pClass = ScriptingManager::getInstance()->getClass(pEntity->getClassName());
    if (pClass == NULL) {
        // LOG: Unknown class name
        return false;
    }
    return pClass->invokeMethod(pEntity, pMethodName, pArguments, numReturns);
}

/**
 * Fills pArguments with the parameters from pLuaState
 *
 * @param  pLuaState the lua state
 * @param  startArg first argument to use (first argument is 0)
 * @param  numArgs number of arguments to use
 * @param  pArguments arguments to be filled
 */
void LuaScriptSystem::fillMethodArguments(lua_State* pLuaState, unsigned int startArg, unsigned int numArgs, MethodArguments* pArguments)
{
    unsigned int argIndex = 0;
    unsigned int i;
    for (i = startArg+1; i<numArgs+startArg+1; i++, argIndex++) {
        switch (lua_type(pLuaState, i)) {
            case LUA_TSTRING:   pArguments->arg[argIndex].s=lua_tostring(pLuaState, i);
                                pArguments->arg[argIndex].m_type=Value::String;
                                break;
            case LUA_TBOOLEAN:  pArguments->arg[argIndex].b=(lua_toboolean(pLuaState, i) == 1);
                                pArguments->arg[argIndex].m_type=Value::Bool;
                                break;

            case LUA_TNUMBER:   pArguments->arg[argIndex].f=(float)lua_tonumber(pLuaState, i);
                                pArguments->arg[argIndex].m_type=Value::Float;
                                break;

            default:            // nothing
                                break;
        }
    }
}
