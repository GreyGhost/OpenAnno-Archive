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

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#define ENTITYMANAGER_ZYKLUSTIME 20

#if defined(_MSC_VER)
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#ifndef _MSC_VER
namespace __gnu_cxx
{
  template<> struct hash< std::string >
  {
    size_t operator()( const std::string& x ) const
    {
      return hash< const char* >()( x.c_str() );
    }
  };
}
#endif

#include <map>
#include <vector>
#include <string>

class Entity;

class EntityManager
{
private:
    typedef std::map<unsigned int, Entity*> EntityUIntMap;
   	// first parameter is the EntityID, second the time of execute
    typedef std::map<unsigned int, unsigned int> UIntUIntMap;
    typedef std::map<unsigned int, std::string> UIntCharMap;
public:
	EntityManager();
    static EntityManager* getInstance();

    Entity* getEntityByName(const char* pName);
    Entity* getEntityById(unsigned int id);
    unsigned int getIdByEntity(Entity* pEntity);
    unsigned int getIdByName(const char *pName);
    bool isEntityRegistered(unsigned int pId);
    bool isEntityRegistered(Entity* pEntity);
	bool isIdValid(unsigned int pId);
    bool addEntity(Entity* pEntity);
    bool removeEntity(Entity* pEntity);
    bool registerEntity(Entity* pEntity, unsigned int ptimeZyklus);
    bool registerEntity(unsigned int pId, unsigned int ptimeZyklus);
    /*
     * @param pId = EntityID, pEntity = Entity
     * @return true if he could successful remove Entity
     * @return true also if the Entity wasn't register
     * @return false only if Entity register, but cann't remove
     */
	bool unregisterEntity(unsigned int pId);
	bool unregisterEntity(Entity* pEntity);
    void getEntityList(std::vector<Entity*>& entityList);
    /*
     * doZyklus()
     * execute doZyklus() of registered Entitys
     * @m_entityTimeMap are the time and the Entity ID
     * @m_timeZyklus are the time 
     */
    void doZyklus();

private:

	UIntCharMap m_entityStringMap;
    EntityUIntMap m_entityUIntMap;
    UIntUIntMap m_entityTimeMap;
    int m_timeZyklus;
};
 
#endif // ENTITYMANAGER_H
