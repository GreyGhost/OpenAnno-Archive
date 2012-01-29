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
# include <SDL_rotozoom.h>
#else
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include <SDL/SDL_rotozoom.h>
#endif
#include <iostream>
#include <physfs.h>
#include "sdlvideosystem.h"
#include "sdlsurface.h"
#include "video/color.h"
#include "io/resourceManager.h"
#include "io/resourceLoader.h"

SDLVideoSystem::SDLVideoSystem() : VideoSystem()
{
}

SDLVideoSystem::~SDLVideoSystem()
{
	SDL_FreeSurface(m_sdlVideoSystem);
	m_sdlVideoSystem = NULL;

	for(cachedZoomedSurfaceList::iterator it = m_cachedZoomedSurfaces.begin();it != m_cachedZoomedSurfaces.end();++it)
	{
		delete it->second;
		it->second = NULL;
	}
	m_cachedZoomedSurfaces.clear();
}

bool SDLVideoSystem::init(int w, int h, int bpp, bool fullscreen)
{
	ResourceManager::getInstance()->addResourceLoader(new GenericResourceLoader<SDLSurface, Resource::SURFACE>());
	ResourceManager::getInstance()->addResourceLoader(new GenericResourceLoader<SDLFont, Resource::FONT>());

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

	if(!reinit(w, h, bpp, fullscreen))
		return false;

	m_defaultFont = ResourceManager::getInstance()->get("font_big.ttf", Resource::FONT).downcast<Font>();

	return true;
}

bool SDLVideoSystem::reinit(int w, int h, int bpp, bool fullscreen)
{
	m_width = w;
	m_height = h;
	m_bpp = bpp;
	int flags = SDL_SWSURFACE;
	if(fullscreen)
		flags |= SDL_FULLSCREEN;

	if(!(m_sdlVideoSystem = SDL_SetVideoMode(m_width, m_height, bpp, flags)))
	{
		std::cout << "SDL_SetVideoMode(" << m_width << ", " << m_height << ", " << bpp << ", " << flags << ") failed: " << SDL_GetError() << std::endl;
		return false;
	}

	for(cachedZoomedSurfaceList::iterator it = m_cachedZoomedSurfaces.begin();it != m_cachedZoomedSurfaces.end();++it)
	{
		delete it->second;
		it->second = NULL;
	}
	m_cachedZoomedSurfaces.clear();

	return true;
}

std::vector<VideoMode> SDLVideoSystem::getAvailableModes() const
{
	std::vector<VideoMode> result;
	int flags = SDL_SWSURFACE | SDL_FULLSCREEN;

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

bool SDLVideoSystem::clear() const
{
	SDL_FillRect(m_sdlVideoSystem, NULL, SDL_MapRGB(m_sdlVideoSystem->format, 0, 0, 0));
	return true;
}

bool SDLVideoSystem::update() const
{
    SDL_UpdateRect(m_sdlVideoSystem, 0, 0, m_width, m_height);

    // Set all cached surfaces to unused and remove unsed ons
	for(cachedZoomedSurfaceList::iterator it = m_cachedZoomedSurfaces.begin();it != m_cachedZoomedSurfaces.end();)
	{
		if(it->second->m_used)
		{
			it->second->m_used = false;
			 ++it;
		}
		else
		{
//			std::cout << "remove cached surface" << it->first << std::endl;
			delete it->second;
			it->second = NULL;
			m_cachedZoomedSurfaces.erase(it++);
		}
	}

	return true;
}

void SDLVideoSystem::drawSurface(const SurfacePtr surface, int x, int y, const Color& color) const
{
	if(x+surface->getWidth() >= 0 && y+surface->getHeight() >= 0 && x < m_width && y < m_height)
	{
		SDL_Rect destRect;
		destRect.x = (Sint16) x;
		destRect.y = (Sint16) y;
		SDL_BlitSurface((surface.downcast<SDLSurface>())->m_surface, NULL, m_sdlVideoSystem, &destRect);
	}
}

void SDLVideoSystem::drawSurfaceScaled(const SurfacePtr surface, int x, int y, float factor, const Color& color) const
{
	if(x+surface->getWidth()*factor >= 0 && y+surface->getHeight()*factor >= 0 && x < m_width && y < m_height)
	{
		SDL_Rect destRect;
		destRect.x = (Sint16) x;
		destRect.y = (Sint16) y;
		cachedZoomedSurfaceList::const_iterator it = m_cachedZoomedSurfaces.find(surface->getName());
		if(it == m_cachedZoomedSurfaces.end())
		{
			SDL_Surface* tmp = zoomSurface((surface.downcast<SDLSurface>())->m_surface, factor, factor, SMOOTHING_OFF);

			SDL_BlitSurface(tmp, NULL, m_sdlVideoSystem, &destRect);
			m_cachedZoomedSurfaces.insert(std::make_pair(surface->getName(), new cachedZoomedSurface(tmp, factor)));
		}
		else if(it->second->m_scale != factor)
		{
			it->second->reuse(zoomSurface((surface.downcast<SDLSurface>())->m_surface, factor, factor, SMOOTHING_OFF), factor);
			SDL_BlitSurface(it->second->m_surface, NULL, m_sdlVideoSystem, &destRect);
		}
		else
		{
			SDL_BlitSurface(it->second->m_surface, NULL, m_sdlVideoSystem, &destRect);
			it->second->m_used = true;
		}
	}
}

void SDLVideoSystem::drawFilledRect(int x, int y, int width, int height, const Color& color) const
{
	SDL_Rect destRect;
	destRect.x = (Sint16) x;
	destRect.y = (Sint16) y;
	destRect.w = (Uint16) width;
	destRect.h = (Uint16) height;

	SDL_FillRect(m_sdlVideoSystem, &destRect, SDL_MapRGBA(m_sdlVideoSystem->format, color.cRed, color.cGreen, color.cBlue, color.cAlpha));
}

void SDLVideoSystem::drawRect(int x, int y, int width, int height, const Color& color) const
{
	if(width < 0)
	{
		x += width;
		width = width*-1;
	}

	if(height < 0)
	{
		y += height;
		height = height*-1;
	}

	if(x >= m_sdlVideoSystem->w || y >= m_sdlVideoSystem->h || x + width < 0 || y + height < 0)
	{
		return;
	}

	if(lock())
	{
		Uint8 *p1 = NULL;
		Uint8 *p2 = NULL;
		Uint8 *p3 = NULL;
		Uint8 *p4 = NULL;
		Uint32 color32 = SDL_MapRGB(m_sdlVideoSystem->format, color.cRed, color.cGreen, color.cBlue);

		if(x < 0)
		{
			width += x;
			x = 0;
			if(y < 0)
			{
				height += y;
				y = 0;
			}
			else
			{
				p1 = (Uint8 *)m_sdlVideoSystem->pixels + y * m_sdlVideoSystem->pitch + x * m_sdlVideoSystem->format->BytesPerPixel;
			}
		}
		else
		{
			if(y < 0)
			{
				height += y;
				y = 0;
			}
			else
			{
				p1 = (Uint8 *)m_sdlVideoSystem->pixels + y * m_sdlVideoSystem->pitch + x * m_sdlVideoSystem->format->BytesPerPixel;
			}
			p3 = (Uint8 *)m_sdlVideoSystem->pixels + y * m_sdlVideoSystem->pitch + x * m_sdlVideoSystem->format->BytesPerPixel;
		}

		if(x + width > m_sdlVideoSystem->w)
		{
			width = m_sdlVideoSystem->w - x;
			if(y + height > m_sdlVideoSystem->h)
			{
				height = m_sdlVideoSystem->h - y;
			}
			else
			{
				((p1 == NULL) ? p1 : p2) = (Uint8 *)m_sdlVideoSystem->pixels + (y + height - 1) * m_sdlVideoSystem->pitch + x * m_sdlVideoSystem->format->BytesPerPixel;
			}
		}
		else
		{
			if(y + height > m_sdlVideoSystem->h)
			{
				height = m_sdlVideoSystem->h - y;
			}
			else
			{
				((p1 == NULL) ? p1 : p2) = (Uint8 *)m_sdlVideoSystem->pixels + (y + height - 1) * m_sdlVideoSystem->pitch + x * m_sdlVideoSystem->format->BytesPerPixel;
			}
			((p3 == NULL) ? p3 : p4) = (Uint8 *)m_sdlVideoSystem->pixels + y * m_sdlVideoSystem->pitch + (x + width - 1) * m_sdlVideoSystem->format->BytesPerPixel;
		}

/*		if(!p1 && !p2 && !p3 && !p4)
		{
				return;
		}*/

		switch(m_sdlVideoSystem->format->BytesPerPixel)
		{
			case 1:
			{
				Uint8 c8 = static_cast<Uint8>(color32);
				if(p1)
				{
					for(int i = width; i > 0; --i)
					{
						*p1 = c8;
						++p1;
					}
					if(p2)
					{
						memcpy(p2, p1-width, width);
					}
				}
				if(p3)
				{
					for(int i = height; i > 0; --i)
					{
							*p3 = c8;
							p3 += m_sdlVideoSystem->pitch;
					}
					if(p4)
					{
						for(int i = height; i > 0; --i)
						{
							*p4 = c8;
							p4 += m_sdlVideoSystem->pitch;
						}
					}
				}
				break;
			}
			case 2:
			{
				Uint16 c16 = static_cast<Uint16>(color32);;
				if(p1)
				{
					for(int i = width; i > 0; --i)
					{
						*(Uint16*)p1 = c16;
						p1 += 2;
					}
					if(p2)
					{
						memcpy(p2, p1-width * 2, width * 2);
					}
				}
				if(p3)
				{
					for(int i = height; i > 0; --i)
					{
							*(Uint16*)p3 = c16;
							p3 += m_sdlVideoSystem->pitch;
					}
					if(p4)
					{
						for(int i = height; i > 0; --i)
						{
							*(Uint16*)p4 = c16;
							p4 += m_sdlVideoSystem->pitch;
						}
					}
				}
				break;
			}
			case 3:
			{
				Uint8 r, g, b;
				#if SDL_BYTEORDER == SDL_BIG_ENDIAN
					r = (color32 >> 16) & 0xff;
					g = (color32 >> 8) & 0xff;
					b = color32 & 0xff;
				#else
					r = (Uint8) (color32 & 0xff);
					g = (Uint8) ((color32 >> 8) & 0xff);
					b = (Uint8) ((color32 >> 16) & 0xff);
				#endif
				if(p1)
				{
					for(int i = width; i > 0; --i)
					{
						p1[0] = r;
						p1[1] = g;
						p1[2] = b;
						p1 += 3;
					}
					if(p2)
					{
						memcpy(p2, p1-width * 3, width * 3);
					}
				}
				if(p3)
				{
					for(int i = height; i > 0; --i)
					{
						p3[0] = r;
						p3[1] = g;
						p3[2] = b;
						p3 += m_sdlVideoSystem->pitch;
					}
					if(p4)
					{
						for(int i = height; i > 0; --i)
						{
							p4[0] = r;
							p4[1] = g;
							p4[2] = b;
							p4 += m_sdlVideoSystem->pitch;
						}
					}
				}
				break;
			}
			case 4:
			{
				if(p1)
				{
					for(int i = width; i > 0; --i)
					{
						*(Uint32*)p1 = color32;
						p1 += 4;
					}
					if(p2)
					{
						memcpy(p2, p1-width * 4, width * 4);
					}
				}
				if(p3)
				{
					for(int i = height; i > 0; --i)
					{
							*(Uint32*)p3 = color32;
							p3 += m_sdlVideoSystem->pitch;
					}
					if(p4)
					{
						for(int i = height; i > 0; --i)
						{
							*(Uint32*)p4 = color32;
							p4 += m_sdlVideoSystem->pitch;
						}
					}
				}
				break;
			}
		}
		unlock();
	}
}

void SDLVideoSystem::drawIsoRect(int x, int y, int width, int height, const Color& color) const
{
		/*TODO*/
}

void SDLVideoSystem::drawFilledIsoRect(int x, int y, int width, int height, const Color& color) const
{
		/*TODO*/
}

void SDLVideoSystem::drawLine(int sX, int sY, int eX, int eY, const Color& color) const
{
	//stupid implementation - but better than none :)
	int dX = sX - eX, dY = sY - eY, i, x, y;
	if(dX < 0)
		dX = -dX;
	if(dY < 0)
		dY = -dY;
	if(dX >= dY) {
		y = (sY < eY ? sY : eY);
		for(i = 0, x = (sX < eX ? sX : eX); i <= dX; ++i, ++x) {
			drawRect(x, y + i * dY / dX + (((i * dY) % dX >= dX / 2) ? 1 : 0), 1, 1, color);
		}
	}
	else {
		x = (sX < eX ? sX : eX);
		for(i = 0, y = (sY < eY ? sY : eY); i <= dY; ++i, ++y) {
			drawRect(x + i * dX / dY + (((i * dX) % dY >= dY / 2) ? 1 : 0), y, 1, 1, color);
		}
	}
}

void SDLVideoSystem::drawText(const std::string& text, int x, int y, const Color& color, Font::Size size, const Rect* const rect) const
{
	if(x < m_width && y < m_height)
		m_defaultFont->drawText(this, text, x, y, color, size, rect);
}

SurfacePtr SDLVideoSystem::createSurface(const std::string& filename)
{
	return (ResourceManager::getInstance()->get(filename, Resource::SURFACE)).downcast<Surface>();
}

VideoMode SDLVideoSystem::getResolution() const
{
	return VideoMode(m_width, m_height, m_bpp);
}

bool SDLVideoSystem::lock() const
{
	if(SDL_MUSTLOCK(m_sdlVideoSystem))
	{
		if(SDL_LockSurface(m_sdlVideoSystem) == -1)
		{
			return false;
		}
	}
	return true;
}

void SDLVideoSystem::unlock() const
{
	if(SDL_MUSTLOCK(m_sdlVideoSystem))
	{
		SDL_UnlockSurface(m_sdlVideoSystem);
	}
}

SDLVideoSystem::cachedZoomedSurface::cachedZoomedSurface(SDL_Surface* surface, float scale) : m_surface(surface), m_scale(scale), m_used(true)
{
}

SDLVideoSystem::cachedZoomedSurface::~cachedZoomedSurface()
{
	SDL_FreeSurface(m_surface);
}

void SDLVideoSystem::cachedZoomedSurface::reuse(SDL_Surface* surface, float scale)
{
	SDL_FreeSurface(m_surface);
	m_surface = surface;
	m_scale = scale;
	m_used = true;
}
