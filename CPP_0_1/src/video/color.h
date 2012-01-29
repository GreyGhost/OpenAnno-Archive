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

#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
	Color() : red(0.0f), green(0.0f), blue(0.0f), alpha(1.0f) { };
	Color(float r, float g, float b, float a = 1.0f) : red(r), green(g), blue(b), alpha(a), cRed(static_cast<unsigned char>(r*255.0f)), cGreen(static_cast<unsigned char>(g*255.0f)), cBlue(static_cast<unsigned char>(b*255.0f)), cAlpha(static_cast<unsigned char>(a*255.0f)) { };
	~Color() { };
	float red, green, blue, alpha;
	unsigned char cRed, cGreen, cBlue, cAlpha;
	
	static Color BLACK;
	static Color WHITE;
	
	static Color RED;
	static Color GREEN;
	static Color BLUE;
	
	static Color GRAY;
	static Color GRAY10;
	static Color GRAY20;
	static Color GRAY30;
	static Color GRAY40;
	static Color GRAY50;
	static Color GRAY60;
	static Color GRAY70;
	static Color GRAY80;
	static Color GRAY90;
	
};

#endif
