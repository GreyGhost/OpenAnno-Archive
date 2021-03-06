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

#ifndef OPENANNO_H
#define OPENANNO_H

#include <stack>

#include "video/videosystem.h"
#include "gui/startscreen.h"

#include "entity.h"
#include "gamestate.h"

#include "audio/audioSystem.h"
#include "jukebox.h"

class OpenAnno : public Entity {
public:
	OpenAnno(int argc, char *argv[]);
	~OpenAnno();
	int run();

	void requestExit();
	void exit();
	void pushGameState(GameState* gamestate);
	GameState* popGameState();

	AudioSystem* getAudioSystem() const;

	virtual const char* getClassName() const;

	float getFPS() { return 1 / fTimeFactor; };

	int fps_limit;
	static float fTimeFactor; // see comment in OpenAnno::run()
private:
	void initGettext(void);
	void draw();
	void handleSDLEvents();
	void invokeLuaKeyEvent(int key, bool isDown);
	void handleInput();

	bool keepRunning;
	AudioSystem* pAudioSystem;

	StartScreen* startscreen;

	int FrameStartingTime;
	int FrameDuration;
	float AverageFrameDuration[5];

	std::stack<GameState*> m_gameStateStack;

	Jukebox* m_jukebox;
};

#endif
