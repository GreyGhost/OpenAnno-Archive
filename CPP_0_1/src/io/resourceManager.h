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

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <vector>
#include "resource.h"
#include "refptr.h"

typedef RefPtr<Resource> ResourcePtr;

class ResourceLoader;

class ResourceManager
{
public:

    static ResourceManager* getInstance();

    void addResourceLoader(ResourceLoader* pResourceLoader);

    ResourcePtr get(const std::string& filename, Resource::TYPE type);
    void freeResource(std::string name);
    bool getStaticDelete(const std::string& name, Resource::TYPE type);
    bool reload(const std::string& name);

private:

    ResourceManager();
    ~ResourceManager();

    static ResourceManager m_instance;

    std::map<std::string, Resource*> m_resources;
    std::vector<ResourcePtr> m_staticDeleteResources;
    std::map<Resource::TYPE, ResourceLoader*> m_resourceLoaders;
};

#endif // RESOURCEMANAGER_H
