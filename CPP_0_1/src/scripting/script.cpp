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

#include <fstream>
#include "script.h"
#include "scriptingManager.h"
#include "luaScriptSystem.h"
#include <physfs.h>

Script::Script(const char* pName) : Resource(pName, SCRIPT, false)
{
}

Script::~Script()
{
    unload();
}

bool Script::load()
{
    if (!m_isLoaded) {
		PHYSFS_file* f;
		if ((f = PHYSFS_openRead(getName().c_str())))
		{
			PHYSFS_sint64 length = PHYSFS_fileLength(f);
            char* pData = new char[(size_t) length+1];
			PHYSFS_sint64 length_read = PHYSFS_read(f, pData, 1, (PHYSFS_uint32) length);
			PHYSFS_close(f);
            pData[length] = 0;
			if (length_read == length)
			{
				ScriptingManager::getInstance()->getScriptSystem()->doBuffer(pData, (unsigned int) length, getName().c_str());
				m_isLoaded = true;
			}
            delete[] pData;
		}
	}
    return m_isLoaded;
}

void Script::unload()
{
    m_isLoaded = false;
}
