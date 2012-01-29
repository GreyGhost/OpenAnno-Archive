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

#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include "resource.h"

class ResourceLoader
{
public:
	virtual ~ResourceLoader() {}
    ResourceLoader(Resource::TYPE type)
    {
        m_type = type;
    }

    Resource::TYPE getType()
    {
        return m_type;
    }
    virtual Resource* getNewInstance(const char* pName)=0;

protected:
    Resource::TYPE m_type;
};

template <class C, Resource::TYPE T>
class GenericResourceLoader : public ResourceLoader
{
public:
    GenericResourceLoader(): ResourceLoader(T) {};

    Resource* getNewInstance(const char* pName)
    {
        return new C(pName);
    };
};

#endif // RESOURCELOADER_H
