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
#ifndef OPENGLSURFACE_H
#define OPENGLSURFACE_H

#include "io/refptr.h"
#include "video/opengl/texture.h"
#include "video/surface.h"

class OpenGLSurface : public Surface
{
friend class OpenGLVideoSystem;
public:
    OpenGLSurface(const std::string& filename);
    ~OpenGLSurface();

    int getWidth() const { return texture.getWidth(); };
    int getHeight() const { return texture.getHeight(); };
    bool load();
    void unload();
    bool reload();

private:
    Texture texture;
};

typedef RefPtr<OpenGLSurface> OpenGLSurfacePtr;

#endif
