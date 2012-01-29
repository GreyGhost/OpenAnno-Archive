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

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32
# include <SDL.h>
# include <SDL_image.h>
#else
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
#endif
#include <GL/gl.h>
#include <iostream>
#include "video/opengl/openglvideosystem.h"
#include "video/color.h"
#include "video/opengl/openglsurface.h"
#include "io/resourceManager.h"
#include "io/resourceLoader.h"


OpenGLVideoSystem::OpenGLVideoSystem() : VideoSystem(), m_defaultFont(NULL)
{
}

OpenGLVideoSystem::~OpenGLVideoSystem()
{
	SDL_FreeSurface(m_sdlVideoSystem);
	m_sdlVideoSystem = NULL;
}

bool OpenGLVideoSystem::init(int w, int h, int bpp, bool fullscreen)
{
    ResourceManager::getInstance()->addResourceLoader(new GenericResourceLoader<OpenGLSurface, Resource::SURFACE>());
    ResourceManager::getInstance()->addResourceLoader(new GenericResourceLoader<OpenGLFont, Resource::FONT>());

	if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
		std::cout << "Couldn't initialize SDL video: " << SDL_GetError() << std::endl;
		return false;
	}
	if(TTF_Init() < 0) {
		std::cout << "Couldn't initialize SDL_ttf: " << TTF_GetError() << std::endl;
		return false;
	}

	SDL_Surface* icon = IMG_Load("icon.png");
	if(!icon)
	{
		std::cout << "Couldn't load window icon: " << IMG_GetError() << std::endl;
	}
	else
	{
		SDL_WM_SetIcon(icon, NULL);
		SDL_FreeSurface(icon);
	}
	SDL_ShowCursor(0);

	return reinit(w, h, bpp, fullscreen);
}

bool OpenGLVideoSystem::reinit(int w, int h, int bpp, bool fullscreen)
{
	for(std::set<OpenGLSurfacePtr>::iterator it = m_surfaces.begin(); it != m_surfaces.end(); ++it)
	{
		(*it)->unload();
	}
	if(m_defaultFont.isNotNull())
	{
		m_defaultFont->unload();
	}
	int flags = SDL_OPENGL;
	if(fullscreen)
		flags |= SDL_FULLSCREEN;

	if(!(m_sdlVideoSystem = SDL_SetVideoMode(w, h, bpp, flags)))
	{
		std::cout << "SDL_SetVideoMode(" << w << ", " << h << ", " << bpp << ", " << flags << ") failed: " << SDL_GetError() << std::endl;
		return false;
	}

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, w, h, 0.0f, -100.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_CULL_FACE);
//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for(std::set<OpenGLSurfacePtr>::iterator it = m_surfaces.begin(); it != m_surfaces.end(); ++it)
	{
		(*it)->load();
	}
	m_width = w;
	m_height = h;
	m_bpp = bpp;

	if(m_defaultFont.isNull())
	{
		m_defaultFont = ResourceManager::getInstance()->get("font_big.ttf", Resource::FONT).downcast<Font>();
	}
	else
	{
		m_defaultFont->load();
	}

	return true;
}

std::vector<VideoMode> OpenGLVideoSystem::getAvailableModes() const
{
	std::vector<VideoMode> result;
	int flags = SDL_FULLSCREEN | SDL_OPENGL;

	SDL_PixelFormat format = *m_sdlVideoSystem->format;
	const SDL_VideoInfo* const video_info = SDL_GetVideoInfo();
	if(video_info != NULL && video_info->vfmt != NULL)
	{
		format.BitsPerPixel = video_info->vfmt->BitsPerPixel;
	}
	const SDL_Rect* const * const modes = SDL_ListModes(&format, flags);
	if(modes == NULL)
	{
		std::cout << "No videomodes available" << std::endl;
	}
	else if(modes == reinterpret_cast<SDL_Rect **>(-1))
	{
		std::cout << "All videomodes available" << std::endl;
	}
	else
	{
		for(int i = 0; modes[i]; ++i)
		{
			result.insert(result.begin(), VideoMode(modes[i]->w, modes[i]->h, format.BitsPerPixel));
		}
	}

	return result;
}

VideoMode OpenGLVideoSystem::getResolution() const
{
	return VideoMode(m_width, m_height, m_bpp);
}

bool OpenGLVideoSystem::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return true;
}

bool OpenGLVideoSystem::update() const
{
/*	for(std::map<std::string, std::vector<drawingRequest> >::iterator it = m_drawingRequests.begin();it != m_drawingRequests.end();++it)
	{
		std::vector<drawingRequest>::iterator it2 = it->second.begin();
		it2->texture->use();
		glBegin(GL_QUADS);

		while(it2 != it->second.end())
		{
			float fX = static_cast<float>(it2->x);
			float fY = static_cast<float>(it2->y);

			glTexCoord2d(0.0, 0.0);
			glVertex2f(fX, fY);

			glTexCoord2d(it2->texture->getTexCoordX(), 0.0);
			glVertex2f(fX+it2->texture->getWidth()*it2->scale, fY);
			glTexCoord2d(it2->texture->getTexCoordX(), it2->texture->getTexCoordY());
			glVertex2f(fX+it2->texture->getWidth()*it2->scale, fY+it2->texture->getHeight()*it2->scale);

			glTexCoord2d(0.0, it2->texture->getTexCoordY());
			glVertex2f(fX, fY+it2->texture->getHeight()*it2->scale);
			++it2;
		}

		glEnd();
		it->second.clear();
	}
	m_drawingRequests.clear();*/
	SDL_GL_SwapBuffers();
	return true;
}

void OpenGLVideoSystem::drawSurface(const SurfacePtr surface, int x, int y, const Color& color) const
{
	if(x+surface->getWidth() >= 0 && y+surface->getHeight() >= 0 && x < m_width && y < m_height)
	{
//		m_drawingRequests[surface->getName()].push_back(drawingRequest(x, y, 1.0f, &surface.downcast<OpenGLSurface>()->texture));
		glColor4ubv(&color.cRed);
		(surface.downcast<OpenGLSurface>())->texture.use();
		glBegin(GL_TRIANGLE_FAN);

		glTexCoord2d(0.0, 0.0);
		glVertex2i(x, y);

		glTexCoord2d((surface.downcast<OpenGLSurface>())->texture.getTexCoordX(), 0.0);
		glVertex2i(x+(surface.downcast<OpenGLSurface>())->texture.getWidth(), y);

		glTexCoord2d((surface.downcast<OpenGLSurface>())->texture.getTexCoordX(), (surface.downcast<OpenGLSurface>())->texture.getTexCoordY());
		glVertex2i(x+(surface.downcast<OpenGLSurface>())->texture.getWidth(), y+(surface.downcast<OpenGLSurface>())->texture.getHeight());

		glTexCoord2d(0.0, (surface.downcast<OpenGLSurface>())->texture.getTexCoordY());
		glVertex2i(x, y+(surface.downcast<OpenGLSurface>())->texture.getHeight());

		glEnd();
	}
}

void OpenGLVideoSystem::drawSurfaceScaled(const SurfacePtr surface, int x, int y, float factor, const Color& color) const
{
	if(x+surface->getWidth()*factor >= 0 && y+surface->getHeight()*factor >= 0 && x < m_width && y < m_height)
	{
//		m_drawingRequests[surface->getName()].push_back(drawingRequest(x, y, factor, &surface.downcast<OpenGLSurface>()->texture));
		glColor4ubv(&color.cRed);
		float fX = static_cast<float>(x);
		float fY = static_cast<float>(y);
		(surface.downcast<OpenGLSurface>())->texture.use();
		glBegin(GL_TRIANGLE_FAN);

		glTexCoord2d(0.0, 0.0);
		glVertex2f(fX, fY);

		glTexCoord2d((surface.downcast<OpenGLSurface>())->texture.getTexCoordX(), 0.0);
		glVertex2f(fX+(surface.downcast<OpenGLSurface>())->texture.getWidth()*factor, fY);
		glTexCoord2d((surface.downcast<OpenGLSurface>())->texture.getTexCoordX(), (surface.downcast<OpenGLSurface>())->texture.getTexCoordY());
		glVertex2f(fX+(surface.downcast<OpenGLSurface>())->texture.getWidth()*factor, fY+(surface.downcast<OpenGLSurface>())->texture.getHeight()*factor);

		glTexCoord2d(0.0, (surface.downcast<OpenGLSurface>())->texture.getTexCoordY());
		glVertex2f(fX, fY+(surface.downcast<OpenGLSurface>())->texture.getHeight()*factor);

		glEnd();
	}
}

void OpenGLVideoSystem::drawFilledRect(int x, int y, int width, int height, const Color& color) const
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_FAN);
	glColor4ubv(&color.cRed);
	glVertex2i(x, y + 1);
	glVertex2i(x+width - 1, y + 1);
	glVertex2i(x+width - 1 , y+height);
	glVertex2i(x, y+height);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void OpenGLVideoSystem::drawRect(int x, int y, int width, int height, const Color& color) const
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINE_LOOP);
	glColor4f(color.red, color.green, color.blue, color.alpha);
	glVertex2i(x, y + 1);
	glVertex2i(x+width - 1, y + 1);
	glVertex2i(x+width - 1 , y+height);
	glVertex2i(x, y+height);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void OpenGLVideoSystem::drawIsoRect(int x, int y, int width, int height, const Color& color) const
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINE_LOOP);
	glColor4f(color.red, color.green, color.blue, color.alpha);
	glVertex2f(x+width/2.0f, y);
	glVertex2f(x+width, y+height/2.0f);
	glVertex2f(x+width/2.0f, y+height);
	glVertex2f(x, y+height/2.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void OpenGLVideoSystem::drawFilledIsoRect(int x, int y, int width, int height, const Color& color) const
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_FAN);
	glColor4ubv(&color.cRed);
	glVertex2f(x+width/2.0f, y);
	glVertex2f(x+width, y+height/2.0f);
	glVertex2f(x+width/2.0f, y+height);
	glVertex2f(x, y+height/2.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void OpenGLVideoSystem::drawLine(int sX, int sY, int eX, int eY, const Color& color) const
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINE);
	glColor4ubv(&color.cRed);
	glVertex3i(sX, sY, 0);
	glVertex3i(eX, eY, 0);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void OpenGLVideoSystem::drawText(const std::string& text, int x, int y, const Color& color, Font::Size size, const Rect* const rect) const
{
	if(x < m_width && y < m_height)
		m_defaultFont->drawText(this, text, x, y, color, size, rect);
}

SurfacePtr OpenGLVideoSystem::createSurface(const std::string& filename)
{
	OpenGLSurfacePtr surface = (ResourceManager::getInstance()->get(filename, Resource::SURFACE)).downcast<OpenGLSurface>();
	if(m_surfaces.find(surface) == m_surfaces.end())
	{
		m_surfaces.insert(surface);
	}
	return surface.downcast<Surface>();
}

