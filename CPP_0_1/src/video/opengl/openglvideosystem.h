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
#ifndef OPENGLSCREEN_H
#define OPENGLSCREEN_H

#include "video/videosystem.h"
#include "video/opengl/openglsurface.h"
#include "video/color.h"
#include "video/opengl/openglfont.h"
#include <vector>
#include <set>
#include <map>

/*struct drawingRequest
{
	drawingRequest(int _x, int _y, float _scale, Texture* _t) : x(_x), y(_y),scale(_scale), texture(_t) { };
	int x, y;
	float scale;
	Texture* texture;
};*/

class OpenGLVideoSystem : public VideoSystem
{
public:
    OpenGLVideoSystem();
    ~OpenGLVideoSystem();
    bool init(int w, int h, int bpp, bool fullscreen);
    bool reinit(int w, int h, int bpp, bool fullscreen);
	std::vector<VideoMode> getAvailableModes() const;
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
private:
	SDL_Surface* m_sdlVideoSystem;
	FontPtr m_defaultFont;
	std::set<OpenGLSurfacePtr> m_surfaces;
//	mutable std::map<std::string, std::vector<drawingRequest> > m_drawingRequests;
};

#endif
