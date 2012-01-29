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

#ifndef METHOD_H
#define METHOD_H

#include "value.h"

class MethodArguments;

class Method
{
public:
    typedef void (*METHOD_FUNCTION)(void* pSelf, MethodArguments* pArguments);

    static const unsigned int MAX_NUM_ARGS    = 4;
    static const unsigned int MAX_NUM_RETURNS = 3;

    // methods with no return value
    Method(const char* pName, METHOD_FUNCTION methodFunction);
    Method(const char* pName, METHOD_FUNCTION methodFunction, Value::Type arg0);
    Method(const char* pName, METHOD_FUNCTION methodFunction, Value::Type arg0, Value::Type arg1);
    Method(const char* pName, METHOD_FUNCTION methodFunction, Value::Type arg0, Value::Type arg1, Value::Type arg2);
    Method(const char* pName, METHOD_FUNCTION methodFunction, Value::Type arg0, Value::Type arg1, Value::Type arg2, Value::Type arg3);

    // methods with 1 return value
    Method(const char* pName, Value::Type ret0, METHOD_FUNCTION methodFunction);
    Method(const char* pName, Value::Type ret0, METHOD_FUNCTION methodFunction, Value::Type arg0);
    Method(const char* pName, Value::Type ret0, METHOD_FUNCTION methodFunction, Value::Type arg0, Value::Type arg1);
    Method(const char* pName, Value::Type ret0, METHOD_FUNCTION methodFunction, Value::Type arg0, Value::Type arg1, Value::Type arg2, Value::Type arg3);

    // methods with 2 return values
    Method(const char* pName, Value::Type ret0, Value::Type ret1, METHOD_FUNCTION methodFunction);
    Method(const char* pName, Value::Type ret0, Value::Type ret1, METHOD_FUNCTION methodFunction, Value::Type arg0);

    // methods with 3 return values
    Method(const char* pName, Value::Type ret0, Value::Type ret1, Value::Type ret2, METHOD_FUNCTION methodFunction);
    Method(const char* pName, Value::Type ret0, Value::Type ret1, Value::Type ret2, METHOD_FUNCTION methodFunction, Value::Type arg0);

    ~Method();

    Value::Type argDef[MAX_NUM_ARGS];
    Value::Type retDef[MAX_NUM_RETURNS];
    unsigned int m_numArgs;
    unsigned int m_numReturns;

    const char* m_pName;
    METHOD_FUNCTION m_methodFunction;

    bool invoke(void* pSelf, MethodArguments* pArguments, int& numReturns);
};

#endif // METHOD_H
