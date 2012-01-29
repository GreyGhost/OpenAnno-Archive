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

#include "method.h"

Method::Method(const char* pName, METHOD_FUNCTION methodFunction)
{
    m_pName = pName;
    m_methodFunction = methodFunction;
    m_numReturns = 0;
    m_numArgs=0;
}

Method::Method(const char* pName, Value::Type ret0, METHOD_FUNCTION methodFunction)
{
    m_pName = pName;
    m_methodFunction = methodFunction;
    retDef[0] = ret0;
    m_numReturns = 1;
    m_numArgs = 0;
}

Method::Method(const char* pName, METHOD_FUNCTION methodFunction, Value::Type arg0)
{
    m_pName = pName;
    m_methodFunction = methodFunction;
    m_numReturns = 0;
    argDef[0] = arg0;
    m_numArgs = 1;
}

Method::Method(const char* pName, METHOD_FUNCTION methodFunction, Value::Type arg0, Value::Type arg1)
{
    m_pName = pName;
    m_methodFunction = methodFunction;
    m_numReturns = 0;
    argDef[0] = arg0;
    argDef[1] = arg1;
    m_numArgs = 2;
}

Method::Method(const char* pName, METHOD_FUNCTION methodFunction, Value::Type arg0, Value::Type arg1, Value::Type arg2)
{
    m_pName = pName;
    m_methodFunction = methodFunction;
    m_numReturns = 0;
    argDef[0] = arg0;
    argDef[1] = arg1;
    argDef[2] = arg2;
    m_numArgs = 3;
}

Method::Method(const char* pName, METHOD_FUNCTION methodFunction, Value::Type arg0, Value::Type arg1, Value::Type arg2, Value::Type arg3)
{
    m_pName = pName;
    m_methodFunction = methodFunction;
    m_numReturns = 0;
    argDef[0] = arg0;
    argDef[1] = arg1;
    argDef[2] = arg2;
    argDef[3] = arg3;
    m_numArgs = 4;
}

Method::Method(const char* pName, Value::Type ret0, METHOD_FUNCTION methodFunction, Value::Type arg0)
{
    m_pName = pName;
    m_methodFunction = methodFunction;
    retDef[0] = ret0;
    m_numReturns = 1;
    argDef[0] = arg0;
    m_numArgs = 1;
}

Method::Method(const char* pName, Value::Type ret0, METHOD_FUNCTION methodFunction, Value::Type arg0, Value::Type arg1)
{
    m_pName = pName;
    m_methodFunction = methodFunction;
    retDef[0] = ret0;
    m_numReturns = 1;
    argDef[0] = arg0;
    argDef[1] = arg1;
    m_numArgs = 2;
}

Method::Method(const char* pName, Value::Type ret0, METHOD_FUNCTION methodFunction, Value::Type arg0, Value::Type arg1, Value::Type arg2, Value::Type arg3)
{
    m_pName = pName;
    m_methodFunction = methodFunction;
    retDef[0] = ret0;
    m_numReturns = 1;
    argDef[0] = arg0;
    argDef[1] = arg1;
    argDef[2] = arg2;
    argDef[3] = arg3;
    m_numArgs = 4;
}

Method::Method(const char* pName, Value::Type ret0, Value::Type ret1, METHOD_FUNCTION methodFunction)
{
    m_pName = pName;
    m_methodFunction = methodFunction;
    retDef[0] = ret0;
    retDef[1] = ret1;
    m_numReturns = 2;
    m_numArgs = 0;
}

Method::Method(const char* pName, Value::Type ret0, Value::Type ret1, METHOD_FUNCTION methodFunction, Value::Type arg0)
{
    m_pName = pName;
    m_methodFunction = methodFunction;
    retDef[0] = ret0;
    retDef[1] = ret1;
    m_numReturns = 2;
    argDef[0] = arg0;
    m_numArgs = 1;
}

Method::Method(const char* pName, Value::Type ret0, Value::Type ret1, Value::Type ret2, METHOD_FUNCTION methodFunction)
{
    m_pName = pName;
    m_methodFunction = methodFunction;
    retDef[0] = ret0;
    retDef[1] = ret1;
    retDef[2] = ret2;
    m_numReturns = 3;
    m_numArgs = 0;
}

Method::Method(const char* pName, Value::Type ret0, Value::Type ret1, Value::Type ret2, METHOD_FUNCTION methodFunction, Value::Type arg0)
{
    m_pName = pName;
    m_methodFunction = methodFunction;
    retDef[0] = ret0;
    retDef[1] = ret1;
    retDef[2] = ret2;
    m_numReturns = 3;
    argDef[0] = arg0;
    m_numArgs = 1;
}

Method::~Method()
{
}

bool Method::invoke(void* pSelf, MethodArguments* pArguments, int& numReturns)
{
    m_methodFunction(pSelf, pArguments);
    numReturns = m_numReturns;
    return true;
}
