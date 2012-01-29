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

#include "entity.h"
#include "entityManager.h"

Entity::Entity(const char* pName, bool registered)
{
    m_id = getUnusedId();
    m_name = pName;
    m_registered = registered;
    if (m_registered) {
        EntityManager::getInstance()->addEntity(this);
    }
}

Entity::~Entity()
{
    if (m_registered) {
        EntityManager::getInstance()->removeEntity(this);
    }
}

const char* Entity::getName() const
{
    return m_name.c_str();
}

void Entity::setName(const char* pName)
{
    m_name = pName;
}

unsigned int Entity::getId() const
{
    return m_id;
}

const char* Entity::getClassName() const
{
    return "Entity";
}

unsigned int Entity::getUnusedId()
{
	// id 0 reserved for errors
    static unsigned int id = 1;
    return id++;
}

void Entity::doZyklusCmd()
{
	
}
