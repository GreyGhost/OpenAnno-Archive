/***************************************************************************
 *   Copyright (C) 2006 Philipp Kerling                                    * 
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
#ifndef ROTATION_H
#define ROTATION_H
#include <string>
class Rotation
{
public:
	enum RotationEnum
	{
		NORTH,
		EAST,
		SOUTH,
		WEST,
		ROTATION_ENUM_SIZE
	};
	Rotation(RotationEnum rotation = NORTH) : m_rotation(rotation) { }	
	inline Rotation(const std::string& name);
	~Rotation() { }
	inline RotationEnum getRotation(void) const
	{
		return m_rotation;
	}
	inline std::string toString(void) const
	{
		switch (m_rotation)
		{
			case NORTH: return "0° North";
			case EAST:  return "90° East";
			case SOUTH: return "180° South";
			case WEST:  return "270° West";
			default:    return "Unknown";
		}
	}
	inline bool operator==(const Rotation& other) const
	{
		return (m_rotation == other.m_rotation);
	}
	inline bool operator!=(const Rotation& other) const
	{
		return (m_rotation != other.m_rotation);
	}
	inline bool operator<(const Rotation& other) const
	{
		return (m_rotation < other.m_rotation); //makes no sense but helps sorting...
	}
	inline Rotation operator+(const Rotation& other) const
	{
		return static_cast<RotationEnum> ((m_rotation + other.m_rotation) & (ROTATION_ENUM_SIZE - 1));
	}
/*
	inline Rotation operator+=(const Rotation& other)
	{
		return static_cast<RotationEnum> ((m_rotation + other.m_rotation) & (ROTATION_ENUM_SIZE - 1));
	}
*/
	inline Rotation operator-(const Rotation& other) const
	{
		return static_cast<RotationEnum> ((m_rotation - other.m_rotation + 4) & (ROTATION_ENUM_SIZE - 1));
	}
/*
	inline Rotation operator-=(const Rotation& other)
	{
		return static_cast<RotationEnum> ((m_rotation - other.m_rotation + 4) & (ROTATION_ENUM_SIZE - 1));
	}
*/
	inline Rotation operator++()
	{
		return static_cast<RotationEnum> ((m_rotation + 1) & (ROTATION_ENUM_SIZE - 1));
	}
	inline Rotation operator++(int)
	{
		Rotation old = *this;
		m_rotation = static_cast<RotationEnum> ((m_rotation + 1) & (ROTATION_ENUM_SIZE - 1));
		return old;
	}
	inline Rotation operator--()
	{
		return static_cast<RotationEnum> ((m_rotation - 1 + 4) & (ROTATION_ENUM_SIZE - 1));
	}
	inline Rotation operator--(int)
	{
		Rotation old = *this;
		m_rotation = static_cast<RotationEnum> ((m_rotation - 1 + 4) & (ROTATION_ENUM_SIZE - 1));
		return old;
	}
	inline friend std::ostream& operator<<(std::ostream& stream, const Rotation& r)
	{
		return stream << r.toString();
	}
private:
	RotationEnum m_rotation;
};

Rotation::Rotation(const std::string& name)
{
	int rot = atoi(name.c_str());
	assert(rot % 90 == 0);
	m_rotation = RotationEnum(rot / 90);
}

#endif /* ROTATION_H */
