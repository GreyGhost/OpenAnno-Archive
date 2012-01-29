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

#include "resource.h"
#include "resourceManager.h"
#include <physfs.h>
#include <assert.h>

Resource::Resource(const std::string& name, TYPE type, bool registered) : m_name(name), m_refCount(0), m_isLoaded(false), m_type(type)
{
}

Resource::~Resource()
{
	assert(m_refCount == 0);
}

bool Resource::reload()
{
    unload();
    return load();
}

bool Resource::isLoaded()
{
    return m_isLoaded;
}

Resource::TYPE Resource::getType()
{
    return m_type;
}

unsigned int Resource::getRefCount() const
{
    return m_refCount;
}

SDL_RWops* Resource::getRWops()
{
	return getRWops(getName());
}

SDL_RWops* Resource::getRWops(const std::string& filename)
{
	// The following lines of this function are
	// Copyright (C) 2006 Matthias Braun <matze@braunis.de>
	// check this as PHYSFS seems to be buggy and still returns a
	// valid pointer in this case
	if(filename == "") {
		std::cout << "Couldn't open file: empty filename" << std::endl;
		return NULL;
	}

    PHYSFS_file* file = (PHYSFS_file*) PHYSFS_openRead(filename.c_str());
    if(!file) {
		std::cout  << "Couldn't open '" << filename << "': "<< PHYSFS_getLastError() << std::endl;
		return NULL;
    }

    SDL_RWops* ops = new SDL_RWops();
    ops->type = 0;
    ops->hidden.unknown.data1 = file;
    ops->seek = RWops_Physfs_Seek;
    ops->read = RWops_Physfs_Read;
    ops->write = 0;
    ops->close = RWops_Physfs_Close;
    return ops;
}

const std::string& Resource::getName() const
{
	return m_name;
}

int Resource::RWops_Physfs_Seek(SDL_RWops *context, int offset, int whence)
{
	// The following lines of this function are
	// Copyright (C) 2006 Matthias Braun <matze@braunis.de>
    PHYSFS_file* file = (PHYSFS_file*) context->hidden.unknown.data1;
    int res;
    switch(whence) {
        case SEEK_SET:
            res = PHYSFS_seek(file, offset);
            break;
        case SEEK_CUR:
            res = PHYSFS_seek(file, PHYSFS_tell(file) + offset);
            break;
        case SEEK_END:
            res = PHYSFS_seek(file, PHYSFS_fileLength(file) + offset);
            break;
        default:
            res = 0;
            assert(false);
            break;
    }
    if(res == 0) {
        std::cout << "Error seeking in file: " << PHYSFS_getLastError() << std::endl;
        return -1;
    }

    return (int) PHYSFS_tell(file);
}

int Resource::RWops_Physfs_Read(SDL_RWops *context, void *ptr, int size, int maxnum)
{
	// The following lines of this function are
	// Copyright (C) 2006 Matthias Braun <matze@braunis.de>
	PHYSFS_file* file = (PHYSFS_file*) context->hidden.unknown.data1;

    int res = PHYSFS_read(file, ptr, size, maxnum);
    return res;
}

int Resource::RWops_Physfs_Write(SDL_RWops *context, const void *ptr, int size, int num)
{
}

int Resource::RWops_Physfs_Close(SDL_RWops *context)
{
	// The following lines of this function are
	// Copyright (C) 2006 Matthias Braun <matze@braunis.de>
    PHYSFS_file* file = (PHYSFS_file*) context->hidden.unknown.data1;

    PHYSFS_close(file);
    delete context;

    return 0;
}

void Resource::free()
{
	ResourceManager::getInstance()->freeResource(getName());
}

