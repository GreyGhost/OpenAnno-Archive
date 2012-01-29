/***************************************************************************
 *   Copyright (C) 2006 by Sven Rech                                       *
 *   svenrech@gmx.de                                                       *
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
#include <stdlib.h>
#include "jukebox.h"
#include "configuration.h"
#include <physfs.h>

Jukebox::Jukebox(const std::string& path) : m_music(NULL)
{
	srand((unsigned int) time(0));
	char** files = PHYSFS_enumerateFiles(path.c_str());
	for (char** it = files; *it != NULL; it++)
	{
		if (std::string(*it).find(".ogg") != std::string::npos)
			m_musicFiles.push_back(path + *it);
	}
	PHYSFS_freeList(files);
}

Jukebox::~Jukebox()
{
}	

void Jukebox::playSong(int num)
{
	if ((num >= 0) && (num < (int)m_musicFiles.size()))
	{
		m_music = AudioSystem::getInstance()->createMusic("JukeboxMusic", m_musicFiles[num]);
		if (m_music.isNull())
		{
			std::cout << "Error loading musicfile " << m_musicFiles[num] << std::endl;
			return;
		}
		if(!AudioSystem::getInstance()->playMusic(m_music, 1))
			std::cout << "Jukebox couldn't play file " << m_musicFiles[num] << "!" << std::endl;
	}
}

void Jukebox::update()
{
	if(!AudioSystem::getInstance()->playingMusic())
	{
		int random = rand() % m_musicFiles.size();
		playSong(random);
	}
}
