/***************************************************************************
 *   Copyright (C) 2006 by Lars-Peter Clausen				   *
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

#ifndef TILE_H
#define TILE_H

#include <string>
#include <map>
#include "io/refptr.h"
#include "video/surface.h"
#include "video/videosystem.h"
#include "rotation.h"
#include "lib/tinyxml/tinyxml.h"

class TileMetaData : public Resource
{
	friend class Tile;
public:
	enum SlopeType
	{
		PLANE = 0,
		FLATHILL,
		STEEPHILL,
		FLATEDGE,
		STEEPEDGE,
		FLATCORNER,
		STEEPCORNER,
		CLIFF
	};
	enum TerrainType
	{
		SEA,
		MOUNTAIN,
		LAND,
		RIVER,
		OBSTACLE
	};
	// FIXME: Should be removed later...
	void debug();
	TileMetaData(const char* pChar);
	virtual ~TileMetaData();
	bool load();
	void unload();
	SurfacePtr getSurface(Rotation rotation, TileMetaData::SlopeType type, float frame = 0);
private:
	class TileFrames
	{
	public:
		TileFrames(SurfacePtr surface, float frames = 0);
		TileFrames();
		void addFrame(SurfacePtr surface, float frames = 0);
		SurfacePtr getSurface(float frame);
		float getFrameCount() const;
	private:
		float m_frameCount;
		std::vector<std::pair<float, SurfacePtr> > m_frames;
	};
	TerrainType m_type;
	Sint8 fertility;
	std::map<std::pair<Rotation, SlopeType>, TileFrames > m_surfaces;
};

typedef RefPtr<TileMetaData> TileMetaDataPtr;

class Tile
{
public:
	Tile(TiXmlElement* element);
	~Tile();

	static Tile getDefaultTile();

	void draw(Rotation camera, int x, int y, float scale, float frame, const VideoSystem* const screen) const;

	Sint8 getFertility() const;
	TileMetaData::TerrainType getTerrain() const;
	
	Uint8 getHeight() const;
	SurfacePtr getSurface(Rotation camera, float frame) const;

private:
	TileMetaDataPtr m_metaData;
	Rotation m_rotation;
	TileMetaData::SlopeType m_slope;
	Uint8 m_height;
};


typedef std::vector<Tile> TileColumn;
typedef std::vector<TileColumn> TileMatrix;

#endif

