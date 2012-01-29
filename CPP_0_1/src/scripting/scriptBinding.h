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

#ifndef SCRIPTBINDING_H
#define SCRIPTBINDING_H

#include "scriptingManager.h"
#include "methodArguments.h"
#include "class.h"
#include "luaScriptSystem.h"
#include "../openanno.h"

#define ADD_SCRIPT_METHOD(methodName) \
    class lua_##methodName##_helper_class { \
    public: \
        static int method(void* pData) \
        { \
            return ScriptingManager::getInstance()->getScriptSystem()->invokeMethod(pData, #methodName); \
        }; \
    }; \
    ScriptingManager::getInstance()->getScriptSystem()->addMethod((int*)lua_##methodName##_helper_class::method, #methodName);

#define ADD_SCRIPT_CONSTRUCTOR(className, methodName) \
    class lua_##methodName##_helper_class { \
    public: \
        static int method(void* pData) \
        { \
            return ScriptingManager::getInstance()->getScriptSystem()->invokeConstructor(pData, #className); \
        }; \
    }; \
    ScriptingManager::getInstance()->getScriptSystem()->addMethod((int*)lua_##methodName##_helper_class::method, #methodName);

static void OpenAnno_requestExit(void* pSelf, MethodArguments* pArgs)
{
    ((OpenAnno*)pSelf)->requestExit();
    // todo
}

static void OpenAnno_printToConsole(void* pSelf, MethodArguments* pArgs)
{
    for (unsigned int i=0; i<4; i++) {
        if (pArgs->arg[i].m_type == Value::Void) {
            break;
        }
        switch (pArgs->arg[i].m_type) {
            case Value::String:
                std::cout << pArgs->arg[i].s;
                break;
            case Value::Float:
                std::cout << pArgs->arg[i].f;
                break;
            case Value::Bool:
                std::cout << pArgs->arg[i].b;
                break;
            default: break;
        };
    }
    std::cout << std::endl;
}

void initScriptBinding()
{
    Class* pOpenAnnoClass = new Class("OpenAnno", NULL, NULL);
    pOpenAnnoClass->addMethod(Method("requestExit", OpenAnno_requestExit));
    pOpenAnnoClass->addMethod(Method("printToConsole", OpenAnno_printToConsole, Value::String, Value::String, Value::String, Value::String));
    ScriptingManager::getInstance()->registerClass(pOpenAnnoClass);

    ADD_SCRIPT_METHOD(requestExit);
    ADD_SCRIPT_METHOD(printToConsole);
}

#endif // SCRIPTBINDING_H
