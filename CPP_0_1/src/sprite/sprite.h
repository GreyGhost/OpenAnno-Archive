/***************************************************************************
 *   Copyright (C) 2006 by Lars-Peter Clausen							   *
 *   lars@laprican.de                                                      *
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

#ifndef SPRITE_H
#define SPRITE_H

#include <map>
#include <string>
#include "spriteanimation.h"

class Sprite
{
public:
	Sprite();
	~Sprite();
	bool load(const std::string& filename);
/*	bool save(const std::string& filename); const*/
	bool setAnimation(const std::string& animationName);
	void update(int delay);
	void draw(int x, int y) const;
private:
	std::map<std::string, SpriteAnimation*> m_animations;
	SpriteAnimation* m_currentAnimation;
};

#endif

