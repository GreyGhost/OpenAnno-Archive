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

#ifndef GRID_H
#define GRID_H

#include <cassert>
#include <vector>

#include "video/point.h"

/** A template class for effectively storing two-dimensional arrays
 *
*/
template<typename T>
class Grid
{
public:
	/** Initialize grid without reserving space
	 *  setSize() has to be called if this constructor is used
	 *  before any other calls are made
	 *
	*/
	Grid();
	
	/** Initialize grid
	 *  @ param size Point describing the size of the grid
	*/
	Grid(const Point& size);
	
	~Grid();

	void setSize(const Point&);
	const Point& getSize() const;

	const T& getElement(const Point&) const;
	void setElement (const Point& position, const T& elem);
	

	/** Check whether a position is on the grid
	 *  @param positon A grid position to check
	 *  @return True, only if the position is on the grid
	 */
	bool isValidPosition(const Point& pos) const;

protected:
	Point m_size;
	std::vector<T> m_tiles;
};

template<typename T> inline
bool Grid<T>::isValidPosition(const Point& p) const
{
	return p.x >= 0 && p.x < m_size.x && p.y >= 0 && p.y < m_size.y;
}

template<typename T> inline
const T& Grid<T>::getElement(const Point& p) const
{
	if (!isValidPosition(p) || m_tiles.empty())
	{
		return 0;
	}

	return m_tiles[p.x + p.y * m_size.x];
}

template<typename T> inline
void Grid<T>::setElement(const Point& p, const T& elem)
{
	assert(m_size.x != -1 || m_size.y != -1);
	if (m_tiles.empty()) {
		m_tiles.resize(m_size.x * m_size.y);
	}
	if (!isValidPosition(p)) {
		return;
	}
	m_tiles[p.x + p.y * m_size.x] = elem;
}


#endif
