/***************************************************************************
 *   Copyright (C) 2006 by Jonathan Sieber                                 *
 *   jonathan_sieber@yahoo.de                                              *
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

#include "grid.h"

template<typename T>
Grid<T>::Grid(const Point& size) : m_size(size)
{
}

template<typename T>
Grid<T>::Grid() : m_size(-1, -1)
{
}


template<typename T>
Grid<T>::~Grid()
{
}

template<typename T>
const Point& Grid<T>::getSize() const
{
	return m_size;
}

template<typename T>
void Grid<T>::setSize(const Point& size)
{
	m_size(size);
}
