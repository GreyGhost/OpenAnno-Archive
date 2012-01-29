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

#include <iostream>
#include <sstream>
#include <cmath>
#include "video/videosystem.h"
#include "tile.h"
#include "lib/tinyxml/tinyxml.h"
#include "io/resourceManager.h"

TileMetaData::TileMetaData(const char* pChar) : Resource(pChar, Resource::TILEMETADATA, true)
{
}

TileMetaData::~TileMetaData()
{
	if (m_isLoaded)
	        unload();
}

void TileMetaData::debug()
{
	std::cout << "Listing of entries: Start" << std::endl;
	for (std::map<std::pair<Rotation, SlopeType>, TileFrames >::iterator i = m_surfaces.begin(); i != m_surfaces.end(); i++)
	{
		std::pair<std::pair<Rotation, SlopeType>, TileFrames> j = (*i);
		std::pair<Rotation, SlopeType> k = j.first;
		Rotation r = k.first;
		SlopeType s = k.second;
		std::cout << "\tRotation: " << r.toString() << ", SlopeType: " << s << std::endl;
	}
	std::cout << "Listing of entires: End" << std::endl;
}

bool TileMetaData::load()
{
	m_isLoaded = true;
	std::cout << "Trying to Load Tile " << getName() << std::endl;
	std::ostringstream sfilename;
	sfilename << "xml/tiles/" << getName() << ".xml";
	std::string filename = sfilename.str();

	TiXmlDocument document;
	if(!document.PHYSFS_LoadFile(filename))
	{
		std::cout << "Couldn't load tile \"" << filename << "\": " << document.ErrorDesc() << std::endl;
		return true;
	}

	if(!document.RootElement() || document.RootElement()->ValueStr() != "tile")
	{
		std::cout << "Couldn't load tile \"" << filename << "\": This is no valid tile" << std::endl;
		return true;
	}

	TiXmlElement* tile = document.RootElement();

	TiXmlElement* elmSlope = tile->FirstChildElement();	
	while (elmSlope)
	{
		if(elmSlope->ValueStr() == "slope")
		{
			SlopeType slope;
			const char* typeAttrib = elmSlope->Attribute("type");
			if (typeAttrib != NULL) {
				std::string slopename = std::string(typeAttrib);
				if (slopename == "plane") {
					slope = PLANE;
				} else if (slopename == "flathill") {
					slope = FLATHILL;
				} else if (slopename == "steephill") {
					slope = STEEPHILL;
				} else if (slopename == "flatedge") {
					slope = FLATEDGE;
				} else if (slopename == "steepedge") {
					slope = STEEPEDGE;
				} else if (slopename == "flatcorner") {
					slope = FLATCORNER;
				} else if (slopename == "steepcorner") {
					slope = STEEPCORNER;
				} else if (slopename == "cliff") {
					slope = CLIFF;
				} else {
					std::cout << "Ignoring unknown entry " << elmSlope->ValueStr() << " in \"" << filename << "\"" << std::endl;
				}
			} else {
				std::cout << "Could not find slope type in " << elmSlope->ValueStr() << " in \"" << filename << "\"" << std::endl;
				slope = PLANE;
			}
			
			TiXmlElement* elmRotation = elmSlope->FirstChildElement();
			while (elmRotation)
			{
				if (elmRotation->ValueStr() == "rotation")
				{
					bool allRotations = std::string(elmRotation->Attribute("angle")) == std::string("all");
					Rotation rotation(elmRotation->Attribute("angle"));
					TiXmlElement* elmFrame = elmRotation->FirstChildElement();
					while (elmFrame)
					{
						if (elmFrame->ValueStr() == "frame")
						{
							double dDuration;
							elmFrame->Attribute("duration", &dDuration);
							float duration = dDuration;
							std::string file = elmFrame->Attribute("image");;
							SurfacePtr frame = VideoSystem::getInstance()->createSurface("images/terrain/gras/day/" + file);
							if(allRotations)
							{
								for (Rotation r = Rotation::NORTH; allRotations || r != Rotation::NORTH; r++)
								{
									allRotations = false;
									std::pair<Rotation, SlopeType> pair = std::make_pair(r, slope);
									std::map<std::pair<Rotation, SlopeType>, TileFrames>::iterator iter = m_surfaces.find(pair);
									if(iter == m_surfaces.end())
									{
										std::pair<std::pair<Rotation, SlopeType>, TileFrames> surface = std::make_pair(pair, TileFrames(frame, duration));
										iter = m_surfaces.insert(surface).first;
									}
									else
									{
										iter->second.addFrame(frame, duration);
									}
								}
							}
							else
							{
								std::pair<Rotation, SlopeType> pair = std::make_pair(rotation, slope);
								std::map<std::pair<Rotation, SlopeType>, TileFrames>::iterator iter = m_surfaces.find(pair);
								if(iter == m_surfaces.end())
								{
									std::pair<std::pair<Rotation, SlopeType>, TileFrames> surface = std::make_pair(pair, TileFrames(frame, duration));
									iter = m_surfaces.insert(surface).first;
								}
								else
								{
									iter->second.addFrame(frame, duration);
								}
							}
						}
						else
						{
							std::cout << "Ignoring unknown entry " << elmFrame->ValueStr() << " in \"" << filename << "\"" << std::endl;
						}
						elmFrame = elmFrame->NextSiblingElement();
					}
				}
				else
				{
					std::cout << "Ignoring unknown entry " << elmRotation->ValueStr() << " in \"" << filename << "\"" << std::endl;
				}
				elmRotation = elmRotation->NextSiblingElement();
			}
		}
		else
		{
			std::cout << "Ignoring unknown entry " << elmSlope->ValueStr() << " in \"" << filename << "\"" << std::endl;
		}
		elmSlope = elmSlope->NextSiblingElement();
	}
	debug();
	return true;
}

void TileMetaData::unload()
{
}

SurfacePtr TileMetaData::getSurface(Rotation rotation, SlopeType slope, float frame)
{
	std::pair<Rotation, SlopeType> pair = std::make_pair(rotation, slope);
	std::map<std::pair<Rotation, SlopeType>, TileFrames>::iterator surface = m_surfaces.find(pair);
	if (surface == m_surfaces.end())
	{
		return NULL;
	}
	return surface->second.getSurface(frame);
}

TileMetaData::TileFrames::TileFrames(SurfacePtr surface, float frames) : m_frameCount(0)
{
	addFrame(surface, frames);
}

TileMetaData::TileFrames::TileFrames() : m_frameCount(0)
{
}

void TileMetaData::TileFrames::addFrame(SurfacePtr surface, float frames)
{
	std::pair<float, SurfacePtr> pair = std::make_pair(frames, surface);
	m_frames.push_back(pair);
	m_frameCount += frames;
}

SurfacePtr TileMetaData::TileFrames::getSurface(float frame)
{
	// Since most tiles have no animation, this optimisation
	// could save some time.
	if (m_frames.size() == 1)
		return m_frames[0].second;

	// This applies a modulo to frame to range it from 0 to m_frameCount
	frame -= (floor(frame / m_frameCount)) * m_frameCount;
	std::vector<std::pair<float, SurfacePtr> >::iterator it = m_frames.begin();
	float sum = 0.0f;
	while (it != m_frames.end())
	{
		sum += it->first;
		if (frame <= sum)
		{
			return it->second;
		}
		it++;
	}
	return NULL;
}

Tile::Tile(TiXmlElement* element)
{
//	std::cout << "  Loading tile..." << std::endl;
	if(!element)
		return;
	if(element->Attribute("angle"))
		m_rotation = Rotation(element->Attribute("angle"));
	else
		m_rotation = Rotation::NORTH;
	if(element->Attribute("slope"))
	{
		std::string slopename = element->Attribute("slope");
		if (slopename == "plane")
			m_slope = TileMetaData::PLANE;
		else if (slopename == "flathill")
			m_slope = TileMetaData::FLATHILL;
		else if (slopename == "steephill")
			m_slope = TileMetaData::STEEPHILL;
		else if (slopename == "flatedge")
			m_slope = TileMetaData::FLATEDGE;
		else if (slopename == "steepedge")
			m_slope = TileMetaData::STEEPEDGE;
		else if (slopename == "flatcorner")
			m_slope = TileMetaData::FLATCORNER;
		else if (slopename == "steepcorner")
			m_slope = TileMetaData::STEEPCORNER;
		else if (slopename == "cliff")
			m_slope = TileMetaData::CLIFF;

	}
	else
		m_slope = TileMetaData::PLANE;

	if(element->Attribute("height"))
		m_height = atoi(element->Attribute("height"));
	else
		m_height = 0;
	ResourcePtr metadata = ResourceManager::getInstance()->get(
		element->Attribute("texture"), Resource::TILEMETADATA);
	m_metaData = metadata.downcast<TileMetaData>();
}

Tile::~Tile()
{
}

Tile Tile::getDefaultTile()
{
	Tile ret(0);
	ret.m_rotation = Rotation::NORTH;
	ret.m_slope = TileMetaData::PLANE;
	ret.m_height = 1;
	ResourcePtr metadata = ResourceManager::getInstance()->get("water", Resource::TILEMETADATA);
	ret.m_metaData = metadata.downcast<TileMetaData>();
	return ret;
}

Uint8 Tile::getHeight() const
{
	return m_height;
}

SurfacePtr Tile::getSurface(Rotation camera, float frame) const
{
	if(m_metaData.isNull())
		return NULL;
	return m_metaData->getSurface(m_rotation - camera, m_slope, frame);
}

void Tile::draw(Rotation camera, int x, int y, float scale, float frame, const VideoSystem* const screen) const
{
	SurfacePtr surface = getSurface(camera, frame);

	if (surface.isNull())
	{
//		std::cout << "Warning: Tried to load unknown Image from Tile " << m_metaData->getName() << " with rotation " << m_rotation.toString() << " and slope type " << m_slope << " !" << std::endl;
		return;
	}

	int y_offset = int((m_height * 16 + surface->getHeight()) * scale);

	screen->drawSurfaceScaled(surface, x, y - y_offset, scale);
}
