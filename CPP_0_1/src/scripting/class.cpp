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

#include "class.h"

Class::Class(const char* pName, CONSTRUCTOR_FUNCTION constructor, Class* pSuperClass)
{
    m_pName = pName;
    m_constructor = constructor;
    m_pSuperClass = pSuperClass;
}

Class::~Class()
{
}

const char* Class::getName() const
{
    return m_pName;
}

void Class::addMethod(Method classMethod)
{
    m_methods.insert(std::make_pair(classMethod.m_pName, classMethod));
}

void* Class::getNewInstance(MethodArguments* pArguments)
{
    if (m_constructor != NULL) {
        return m_constructor(pArguments);
    } else {
        return NULL;
    }
}

bool Class::invokeMethod(void* pSelf, const char* pMethodName, MethodArguments* pArguments, int& numReturns)
{
    std::map<std::string, Method>::iterator it = m_methods.find(pMethodName);
    if (it != m_methods.end()) {
        return it->second.invoke(pSelf, pArguments, numReturns);
    } else {
        if (m_pSuperClass != NULL) {
            return m_pSuperClass->invokeMethod(pSelf, pMethodName, pArguments, numReturns);
        }
    }
    return false;
}
