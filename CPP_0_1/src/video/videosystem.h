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
#ifndef SCREEN_H
#define SCREEN_H

#include "video/surface.h"
#include "video/color.h"
#include "video/font.h"
#include <vector>

class VideoMode
{
public:
	VideoMode() : m_width(-1), m_height(-1), m_bpp(-1) { };
	VideoMode(int width, int height, int bpp) : m_width(width), m_height(height), m_bpp(bpp) { };
	~VideoMode() { };
	bool operator ==(const VideoMode& videomode) { return videomode.getWidth() == m_width && videomode.getHeight() == m_height && videomode.getBpp() == m_bpp; };
	bool operator !=(const VideoMode& videomode) { return videomode.getWidth() != m_width || videomode.getHeight() != m_height || videomode.getBpp() != m_bpp; };
	int getWidth() const { return m_width; };
	int getHeight() const { return m_height; };
	int getBpp() const { return m_bpp; };

private:
	int m_width, m_height;
	int m_bpp;
};

typedef std::vector<VideoMode> VideoModes;

class VideoSystem
{
public:
	static void initInstance(VideoSystem* instance);
	static VideoSystem* getInstance();
	static void freeInstance();

    VideoSystem();
    virtual ~VideoSystem();
    virtual bool init(int w, int h, int depth, bool fullscreen) = 0;
    virtual bool reinit(int w, int h, int depth, bool fullscreen) = 0;
    virtual bool clear() const = 0;
    virtual bool update() const = 0;
    virtual void drawSurface(const SurfacePtr surface, int x, int y, const Color& color = Color::WHITE) const = 0;
    virtual void drawSurfaceScaled(const SurfacePtr surface, int x, int y, float factor, const Color& color = Color::WHITE) const = 0;
    virtual void drawFilledRect(int x, int y, int width, int height, const Color& color) const = 0;
    virtual void drawRect(int x, int y, int width, int height, const Color& color) const = 0;
    virtual void drawIsoRect(int x, int y, int width, int height, const Color& color) const = 0;
    virtual void drawFilledIsoRect(int x, int y, int width, int height, const Color& color) const = 0;
    virtual void drawLine(int sX, int sY, int eX, int eY, const Color& color) const = 0;
	virtual void drawText(const std::string& bsditext, int x, int y, const Color& color, Font::Size = Font::SIZE_MIDDLE, const Rect* const rect = NULL) const = 0;
	virtual int getWidth() const { return m_width; };
	virtual int getHeight() const { return m_height; };
	virtual int getBpp() const { return m_bpp; };
	virtual const FontPtr getDefaultFont() const = 0;
	virtual SurfacePtr createSurface(const std::string& filename) = 0;
	virtual std::vector<VideoMode> getAvailableModes() const = 0;
	virtual VideoMode getResolution() const = 0;
    static VideoSystem* m_instance;
protected:
	int m_width, m_height;
	int m_bpp;
};

#endif
