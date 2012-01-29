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
#ifndef SDLSCREEN_H
#define SDLSCREEN_H

#include <map>

#include "video/videosystem.h"
#include "sdlsurface.h"
#include "video/color.h"
#include "sdlfont.h"

class SDLVideoSystem : public VideoSystem
{
friend class SDLFont;

	class cachedZoomedSurface
	{
		public:
			cachedZoomedSurface(SDL_Surface* surface, float scale);
			~cachedZoomedSurface();
			void reuse(SDL_Surface* surface, float scale);
			SDL_Surface* m_surface;
			float m_scale;
			 // This is set to false at end of each frame and to true if it is drawn.
			 // If it is still false at the end of the next frame the cache is removed
			bool m_used;
	};
	typedef std::map<std::string, cachedZoomedSurface*> cachedZoomedSurfaceList;
public:
    SDLVideoSystem();
    ~SDLVideoSystem();
    bool init(int w, int h, int depth, bool fullscreen);
    bool reinit(int w, int h, int depth, bool fullscreen);
    VideoModes getAvailableModes() const;
    VideoMode getResolution() const;
    bool clear() const;
    bool update() const;
    void drawSurface(const SurfacePtr surface, int x, int y, const Color& color = Color::WHITE) const;
    void drawSurfaceScaled(const SurfacePtr surface, int x, int y, float factor, const Color& color = Color::WHITE) const;
    void drawFilledRect(int x, int y, int width, int height, const Color& color) const;
    void drawRect(int x, int y, int width, int height, const Color& color) const;
    void drawIsoRect(int x, int y, int width, int height, const Color& color) const;
    void drawFilledIsoRect(int x, int y, int width, int height, const Color& color) const;
    void drawLine(int sX, int sY, int eX, int eY, const Color& color) const;
	void drawText(const std::string& text, int x, int y, const Color& color, Font::Size size = Font::SIZE_MIDDLE, const Rect* const rect = NULL) const;
	const FontPtr getDefaultFont() const { return m_defaultFont; };
	SurfacePtr createSurface(const std::string& filename);
	void freeSurface(SurfacePtr surface);
	bool lock() const;
	void unlock() const;
private:
	SDL_Surface* m_sdlVideoSystem;
	FontPtr m_defaultFont;
	mutable cachedZoomedSurfaceList m_cachedZoomedSurfaces;
};

#endif
