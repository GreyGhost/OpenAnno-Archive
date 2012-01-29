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

#include <iostream>
#include "exception.h"
#include "resourceManager.h"
#include "resource.h"
#include "resourceLoader.h"

ResourceManager ResourceManager::m_instance;

ResourceManager* ResourceManager::getInstance()
{
    return &m_instance;
}

void ResourceManager::addResourceLoader(ResourceLoader* pResourceLoader)
{
    m_resourceLoaders.insert(std::make_pair(pResourceLoader->getType(), pResourceLoader));
}

ResourcePtr ResourceManager::get(const std::string& name, Resource::TYPE type)
{
    // check if resource is already loaded
    std::map<std::string, Resource*>::iterator pos = m_resources.find(name);
    if (pos != m_resources.end()) {
        ResourcePtr pResource = pos->second;

        if (pResource->getType() == type) {
            return pResource;
        } else {
            //LOG: error, cant load resource with same name but different type
        }
    } else {
        std::map<Resource::TYPE, ResourceLoader*>::iterator loader = m_resourceLoaders.find(type);
        if (loader != m_resourceLoaders.end()) {
            Resource* pResource = loader->second->getNewInstance(name.c_str());
            if (pResource != NULL) {
                if (pResource->load()) {
					m_resources.insert(std::make_pair(pResource->getName(), pResource));
                    return pResource;
                } else {
                    //LOG: resource loading failed
                    delete pResource;
                }
            }
        } else {
            //LOG: no loader found to load resource
        }
    }
    throw FileLoadException(name, "File not found");
}

bool ResourceManager::getStaticDelete(const std::string& name, Resource::TYPE type)
{
    ResourcePtr pResource = get(name, type);
    if (pResource.isNotNull()) {
        if (pResource->getRefCount() == 1) {
            // the resource was just created
            m_staticDeleteResources.push_back(pResource);
        }
        return true;
    }
    return false;
}

bool ResourceManager::reload(const std::string& name)
{
    std::map<std::string, Resource*>::iterator pos = m_resources.find(name);
    if (pos != m_resources.end()){
        Resource* pResource = pos->second;
        pResource->reload();
        return true;
    }
    return false;
}

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
    std::map<Resource::TYPE, ResourceLoader*>::iterator it;
    for (it = m_resourceLoaders.begin(); it != m_resourceLoaders.end(); ++it) {
        delete it->second;
    }
    m_resourceLoaders.clear();

    std::vector<ResourcePtr>::iterator staticIt;
    for (staticIt = m_staticDeleteResources.begin(); staticIt != m_staticDeleteResources.end(); ++staticIt) {
        while ((*staticIt)->getRefCount() > 1) {
            (*staticIt)->removeRef();
        }
        //delete *staticIt;
    }
    m_staticDeleteResources.clear();


    for(std::map<std::string, Resource*>::iterator it = m_resources.begin(); it != m_resources.end(); ++it)
	{
		std::cout << "Unfreed resource \"" << it->first << "\", RefCout: " << it->second->getRefCount() << std::endl;
        while (it->second->getRefCount() > 1)
        {
            it->second->removeRef();
        }
		it->second->removeRef();
	}
	m_resources.clear();
}

void ResourceManager::freeResource(std::string name)
{
    std::map<std::string, Resource*>::iterator pos = m_resources.find(name);
    if (pos != m_resources.end())
    {
    	assert(pos->second->getRefCount() == 0);
    	delete pos->second;
    	m_resources.erase(pos);
    	std::cout << "Freed Resource \"" << name << "\"" << std::endl;
    }
}
