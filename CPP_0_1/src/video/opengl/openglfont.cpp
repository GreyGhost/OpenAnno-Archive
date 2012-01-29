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


#include <iostream>
#include <string>
#ifdef _WIN32
# include <SDL_ttf.h>
#else
# include <SDL/SDL_ttf.h>
#endif
#include "video/opengl/openglfont.h"
#include "video/sdl/sdlvideosystem.h"

OpenGLFont::OpenGLFont(const std::string& filename) : Font(filename), texture(NULL), ttfFont(NULL)
{
}


OpenGLFont::~OpenGLFont()
{
	if (m_isLoaded)
	{
		unload();
	}
}

void OpenGLFont::drawText(const VideoSystem* const screen, std::string text, int x, int y, const Color& color, Size size, const Rect* const rect) const
{
	if(!ttfFont)
	{
		return;
	}

	int offsetX = 0;
	int offsetY = 0;

	float rectX, rectY;
	float rectWidth, rectHeight;
	float xTexMin, xTexMax, yTexMin, yTexMax;

	if(rect)
	{
		offsetX = -rect->x;
	}

	float factor = m_fontSizes[size] / static_cast<float>(loadedFontSize);

	float height = texture->getHeight() * factor;

	texture->use();

	glColor4f(color.red, color.green, color.blue, color.alpha);

	glBegin(GL_QUADS);

	for(std::string::size_type i = 0; i < text.size(); ++i)
	{
		if(text[i] == '\n')
		{
			offsetY += static_cast<int>(TTF_FontLineSkip(ttfFont) * factor);
			offsetX = 0;
		}

		if(text[i] < 32)
			continue;

		int c = text[i] - 32;

		rectX = static_cast<float>(x + offsetX);
		rectY = static_cast<float>(y + offsetY);
		rectWidth = glyphData[c].width * factor;
		rectHeight = height;
		xTexMin = glyphData[c].xTexMin;
		xTexMax = glyphData[c].xTexMax;
		yTexMin = 0.0f;
		yTexMax = texture->getTexCoordY();


		if(rect)
		{
			if(offsetX < 0)
			{
				if(static_cast<int>(offsetX + rectWidth) < 0)
				{
					offsetX += static_cast<int>(glyphData[c].width * factor);
					continue;
				}
				xTexMin = xTexMax - (xTexMax - xTexMin) * ((rectWidth + offsetX) / rectWidth);
				rectWidth = rectWidth + offsetX;
				rectX = static_cast<float>(x);

			}
			if(static_cast<int>(offsetX + rectWidth) >= rect->width)
			{
				if(offsetX >= rect->width)
				{
					break;
				}

				float newWidth = static_cast<float>(rect->width - offsetX);
				xTexMax = ((xTexMax - xTexMin) * (newWidth / rectWidth)) + xTexMin;

				rectWidth = newWidth;
			}
		}

		glTexCoord2d(xTexMin, yTexMin);
		glVertex3f(rectX, rectY, 0);

		glTexCoord2d(xTexMax, yTexMin);
		glVertex3f(rectX + rectWidth, rectY, 0);

		glTexCoord2d(xTexMax, yTexMax);
		glVertex3f(rectX + rectWidth, rectY + rectHeight, 0);

		glTexCoord2d(xTexMin, yTexMax);
		glVertex3f(rectX, rectY + rectHeight, 0);

		offsetX += static_cast<int>(glyphData[c].width * factor);
	}
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

int OpenGLFont::getSingleLineTextWidth(const std::string& text, float factor) const
{
	int width = 0;

	std::string::size_type i = text.size();
	do
	{
		--i;
		if(text[i] < 32)
			continue;
		width += static_cast<int>(glyphData[text[i]-32].width * factor);
	}
	while(i > 0);


	return width;
}

int OpenGLFont::getTextWidth(const std::string& text, Size size) const
{
	if(!ttfFont)
	{
		return -1;
	}

	int maxWidth = 0;
	int width = 0;
	std::string::size_type oldPos = 0;
	std::string::size_type pos = text.find("\n");

	float factor = m_fontSizes[size] / static_cast<float>(loadedFontSize);

	while(pos != std::string::npos)
	{
		width = getSingleLineTextWidth(text.substr(oldPos, pos-oldPos).c_str(), factor);
		if(width > maxWidth)
		{
			maxWidth = width;
		}
		oldPos = pos+1;
		pos = text.find("\n", pos+1);
	}
	if(oldPos < text.size())
	{
		width = getSingleLineTextWidth(text.substr(oldPos).c_str(), factor);
	}
	if(width > maxWidth)
	{
		maxWidth = width;
	}
	return maxWidth;
}

int OpenGLFont::getTextHeight(const std::string& text, Size size) const
{
	if(!ttfFont)
	{
		return -1;
	}

	if(!text.size())
	{
		return 0;
	}

	int lines = 1;
	std::string::size_type pos = text.find("\n");
	while(pos != std::string::npos)
	{
		++lines;
		pos = text.find("\n", pos+1);
	}
	return lines * TTF_FontLineSkip(ttfFont) * m_fontSizes[size] / loadedFontSize;
}

bool OpenGLFont::load()
{
	if(m_isLoaded)
	{
		return true;
	}

	texture = new Texture();

	SDL_RWops* fileOps = getRWops();
	if(!fileOps)
	{
		std::cout << "Couldn't load TTF-font file: " << TTF_GetError() << std::endl;
		return false;
	}
	ttfFont = TTF_OpenFontRW(fileOps, 1, loadedFontSize);

	if(!ttfFont)
	{
		std::cout << "Couldn't load TTF-font file: " << TTF_GetError() << std::endl;
		return false;
	}

	SDL_Surface *tempSurface = NULL, *textureSurface = NULL;
	SDL_Rect destRect;
	destRect.y = 0;

	SDL_Color color = { 255,255,255};

	char string[2];
	string[1] = 0;

	int newWidth = 0, newHeight = 0;

	for(int i = 95; i >= 0; --i)
	{
		string[0] = static_cast<char>(i)+32;
		SDL_Surface* surface = TTF_RenderUTF8_Blended(ttfFont, string, color);
		if(!surface)
		{
			std::cout << "Couldn't create font surface: " << SDL_GetError() << std::endl;
			return false;
		}
		SDL_SetAlpha(surface, 0, 0);

		if(textureSurface)
		{
			newWidth = surface->w + textureSurface->w;
			newHeight = (textureSurface->h > surface->h ? textureSurface->h : surface->h);
		}
		else
		{
			newWidth = surface->w;
			newHeight = surface->h;
		}
		#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			tempSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, newWidth, newHeight, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
		#else
			tempSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, newWidth, newHeight, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
		#endif
		SDL_SetAlpha(tempSurface, 0, 0);

		if(textureSurface)
		{
			if(SDL_BlitSurface(textureSurface, NULL, tempSurface, NULL) != 0)
			{
				std::cout << "SDL_BlitSurface failed: " << SDL_GetError() << std::endl;
			}
			destRect.x = (Sint16) textureSurface->w;
			destRect.w = (Sint16) surface->w;
			destRect.h = (Sint16) surface->h;
			if(SDL_BlitSurface(surface, NULL, tempSurface, &destRect) != 0)
			{
				std::cout << "SDL_BlitSurface failed: " << SDL_GetError() << std::endl;
			}
			SDL_FreeSurface(textureSurface);
		}
		else
		{
			if(SDL_BlitSurface(surface, NULL, tempSurface, NULL) != 0)
			{
				std::cout << "SDL_BlitSurface failed: " << SDL_GetError() << std::endl;
			}
		}

		textureSurface = tempSurface;

		glyphData[i].width = surface->w;
		glyphData[i].xTexMin = static_cast<float>(newWidth - surface->w);
		glyphData[i].xTexMax = static_cast<float>(newWidth);

		SDL_FreeSurface(surface);

/*		if(!glyphs[i].createFromSdlSurface(surface))
		{
			std::cout << "Failed to create font texture" << std::endl;
		}*/
	}

	if(!texture->createFromSdlSurface(textureSurface))
	{
		std::cout << "Failed to create font texture" << std::endl;
	}

	for(int i = 95; i >= 0; --i)
	{
		glyphData[i].xTexMin = glyphData[i].xTexMin / static_cast<float>(texture->getWidth()) * texture->getTexCoordX();
		glyphData[i].xTexMax = glyphData[i].xTexMax / static_cast<float>(texture->getWidth()) * texture->getTexCoordX();
	}

	return (m_isLoaded = true);
}

void OpenGLFont::unload()
{
	if(m_isLoaded)
	{
		if(ttfFont)
		{
			TTF_CloseFont(ttfFont);
		}

		delete texture;
		texture = NULL;
		m_isLoaded = false;
	}
}
