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

#include "entityManager.h"
#include "entity.h"

EntityManager::EntityManager()
{
	m_timeZyklus = 0;
}

EntityManager* EntityManager::getInstance()
{
    static EntityManager instance;
    return &instance;
}

Entity* EntityManager::getEntityByName(const char* pName)
{

    return NULL;
}

Entity* EntityManager::getEntityById(unsigned int id)
{
    EntityUIntMap::iterator itr = m_entityUIntMap.find(id);

    if (itr != m_entityUIntMap.end()) 
    {
        return (*itr).second;
    }
    return NULL;
}

unsigned int EntityManager::getIdByEntity(Entity* pEntity)
{
	EntityUIntMap::iterator itr;
	for(itr = m_entityUIntMap.begin();itr != m_entityUIntMap.end(); itr++)
	{
		if(pEntity == itr->second)
		{
			return itr->first;
		}
	}
	return 0;
}

unsigned int EntityManager::getIdByName(const char *pName)
{
	UIntCharMap::iterator itr;
	for(itr = m_entityStringMap.begin();itr != m_entityStringMap.end(); itr++)
	{
		if(pName == itr->second)
		{
			return itr->first;
		}
	}
	return 0;
}

bool EntityManager::addEntity(Entity* pEntity)
{
	// TODO add/edit return value
    if (getEntityByName(pEntity->getName()) != NULL)
    {
        return false;
    }

    m_entityStringMap.insert(std::make_pair(pEntity->getId(),pEntity->getName()));
    m_entityUIntMap.insert(std::make_pair(pEntity->getId(), pEntity));
    return true;
}

bool EntityManager::removeEntity(Entity* pEntity)
{
	unsigned int id = getIdByEntity(pEntity);
	if(id == 0)
		return false;
	// erase Maps - if failed return false
	// TODO after one failed, he tried to erase 
    EntityUIntMap::iterator itr = m_entityUIntMap.find(id);
    if (itr != m_entityUIntMap.end()) 
        m_entityUIntMap.erase(itr);
    else
    	return false;
    if(isEntityRegistered(id))
    {
	    UIntUIntMap::iterator itr2 = m_entityTimeMap.find(id);
	    if (itr2 != m_entityTimeMap.end())
	    	m_entityTimeMap.erase(itr2);
	    else
	    	return false;
    }
    UIntCharMap::iterator itr3 = m_entityStringMap.find(id);
    if (itr3 != m_entityStringMap.end())
    	m_entityStringMap.erase(itr3);
    else
    	return false;
    
    return true;
    	
}

void EntityManager::getEntityList(std::vector<Entity*>& entityList)
{
	// TODO EntityManager::getEntityList
}

void EntityManager::doZyklus()
{
	m_timeZyklus += ENTITYMANAGER_ZYKLUSTIME ;
	UIntUIntMap::iterator timeiterator = m_entityTimeMap.begin();
	for(;timeiterator != m_entityTimeMap.end(); timeiterator++)
	{
		// search in m_entityTimeMap for a zyklus, that should be run now !
		// we get here the ID of Entity
		if(m_timeZyklus % timeiterator->second == 0)
		{
			// warning you get a NULLPOINTER if the ID doesn't exist
			getEntityById(timeiterator->first)->doZyklusCmd();
		}
	}
}

bool EntityManager::registerEntity(Entity* pEntity, unsigned int pTimeZyklus)
{
	// TODO add return value
	unsigned int id = EntityManager::getIdByEntity(pEntity);
	if(id == 0)
	{
		addEntity(pEntity);
		id = EntityManager::getIdByEntity(pEntity);
	}
	m_entityTimeMap.insert(std::make_pair(id, pTimeZyklus));
	return true;
}

bool EntityManager::registerEntity(unsigned int pId, unsigned int pTimeZyklus)
{
	// TODO add return var
	if(isIdValid(pId) == true)
	{
		m_entityTimeMap.insert(std::make_pair(pId, pTimeZyklus));
	}
	return true;
}

bool EntityManager::isEntityRegistered(unsigned int pId)
{
	UIntUIntMap::iterator itr = m_entityTimeMap.find(pId);
	if(itr != m_entityTimeMap.end())
		return true;
	else
		return false;
}

bool EntityManager::isEntityRegistered(Entity* pEntity)
{
	return isEntityRegistered(getIdByEntity(pEntity));
}

bool EntityManager::isIdValid(unsigned int pId)
{
	EntityUIntMap::iterator itr = m_entityUIntMap.find(pId);
	if(itr != m_entityUIntMap.end())
		return true;
	else
		return false;
}

bool EntityManager::unregisterEntity(unsigned int pId)
{
	if(isEntityRegistered(pId))
    {
	    UIntUIntMap::iterator itr2 = m_entityTimeMap.find(pId);
	    if (itr2 != m_entityTimeMap.end())
	    	m_entityTimeMap.erase(itr2);
	    else
	    	return false;
    }
    return true;
}
bool EntityManager::unregisterEntity(Entity* pEntity)
{
	return unregisterEntity(getIdByEntity(pEntity));
}
