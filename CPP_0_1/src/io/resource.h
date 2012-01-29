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

#ifndef RESOURCE_H
#define RESOURCE_H

#ifdef _WIN32
# include <SDL_rwops.h>
#else
# include <SDL/SDL_rwops.h>
#endif
#include "entity.h"
#include "platform.h"

class Resource
{

	friend class ResourceManager;
public:
	enum TYPE {
		SURFACE,
		SAMPLE,
		MUSIC,
		SCRIPT,
		TILEMETADATA,
		FONT,
		NUM_TYPES
	};

	static const char* TYPE_NAMES[NUM_TYPES];

public:

	Resource(const std::string& name, TYPE type, bool registered);

	bool reload();
	virtual bool load() = 0;
	virtual void unload() = 0;

	bool isLoaded();

	TYPE getType();

	void addRef() FORCEINLINE
	{
		++m_refCount;
	}

	void removeRef() FORCEINLINE
	{
		--m_refCount;
		if (m_refCount == 0) {
			free();
		}
	}

	unsigned int getRefCount() const;

	const std::string& getName() const;

protected:

	virtual ~Resource();

	unsigned int m_refCount;
	bool m_isLoaded;
	TYPE m_type;
	std::string m_name;

	SDL_RWops* getRWops();
	SDL_RWops* getRWops(const std::string& filename);
private:
	static int RWops_Physfs_Seek(SDL_RWops *context, int offset, int whence);
	static int RWops_Physfs_Read(SDL_RWops *context, void *ptr, int size, int maxnum);
	static int RWops_Physfs_Write(SDL_RWops *context, const void *ptr, int size, int num);
	static int RWops_Physfs_Close(SDL_RWops *context);

	void free();
};

#endif // RESOURCE_H
