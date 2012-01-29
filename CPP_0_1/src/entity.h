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

#ifndef ENTITY_H
#define ENTITY_H

#include <string>

class Entity
{
public:
    Entity(const char* pName, bool registered);
    virtual ~Entity();

    const char* getName() const;
    void setName(const char* pName);
    unsigned int getId() const;

    virtual const char* getClassName() const;
    /**
     * doZyklusCmd called by EntityManager::doZyklus
     * this function runs lua scripts
     */
	void doZyklusCmd();
private:
	/**
	 * all entitys get an unique id
	 */
    static unsigned int getUnusedId();

    std::string m_name;
    unsigned int m_id;
    bool m_registered;
};

#endif // ENTITY_H
