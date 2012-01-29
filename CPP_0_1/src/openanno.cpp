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

#include "configInclude.h"
#include "gettext.h"

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#include <tchar.h>
#else
#include "config.h"
#endif

#ifdef _WIN32
# include <SDL.h>
# include <SDL_image.h>
#else
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
#endif
#include <iostream>
#include <stdlib.h>
#ifndef __arm__
	#include "video/opengl/openglvideosystem.h"
#endif
#include "video/sdl/sdlvideosystem.h"
#include "openanno.h"
#include "gui/startscreen.h"

#include "io/resourceManager.h"
#include "io/resourceLoader.h"

#include "scripting/scriptBinding.h"
#include "scripting/script.h"

#include "gui/widgetmanager.h"
#include "gui/events/eventmousemove.h"
#include "gui/events/eventmouseclick.h"
#include "configuration.h"

#include "audio/audioSystem.h"
#include "audio/music.h"
#include "ingame/tile.h"

#include <physfs.h>
#include <exception>

int main(int argc, char *argv[])
{
	OpenAnno openanno(argc, argv);
	return openanno.run();
}

float OpenAnno::fTimeFactor = 0.0f;

void OpenAnno::initGettext()
{
	setlocale( LC_ALL, "" );
#ifndef WIN32
    std::cout << "Gettext package: " << GETTEXT_PACKAGE << ", locale directory: " << PACKAGE_LOCALE_DIR << ", NLS enabled: " << ENABLE_NLS << std::endl;
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif
	std::cout << _("Language: English") << std::endl;
}

OpenAnno::OpenAnno(int argc, char *argv[]) : Entity("OpenAnno", true), keepRunning(true)
{
	initGettext();

	if (PHYSFS_init(argv[0]))
		std::cout << "Physfs successfully initialized!" << std::endl;

	//Search for the data source:
	PHYSFS_addToSearchPath(".", 0);
	if (PHYSFS_exists("data"))
	{
		if (PHYSFS_addToSearchPath("data", 0))
			std::cout << "Successfully added the data directory to the physfs search path" << std::endl;
	}
	else if (PHYSFS_exists("data.zip"))
	{
		if (PHYSFS_addToSearchPath("data.zip", 0))
			std::cout << "Successfully added the data.zip archive to the physfs search path" << std::endl;
	}
	else
	{
		std::cout << "Error: Data files do not exist!" << std::endl;
		throw std::exception();
	}
	PHYSFS_removeFromSearchPath(".");


#ifdef WIN32
	TCHAR pszPath[MAX_PATH+1];
	std::string userdir;

	HMODULE hmod = LoadLibrary(_T("shell32.dll"));
	if (GetProcAddress(hmod, "SHGetFolderPathA"))
	{
#ifndef SHGFP_TYPE_CURRENT
#define SHGFP_TYPE_CURRENT 0
#endif
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, pszPath)))
			userdir = pszPath;
			if (userdir[userdir.size()-1] != '\\')
				userdir.append("\\");
	}
	FreeLibrary(hmod);
	std::string dirname = "OpenAnno";
#else
	std::string userdir = PHYSFS_getUserDir();
	std::string dirname = ".openanno";
#endif

	std::string oadir = userdir + dirname;
	//Create the ~/.openanno dir, and add it to physfs search path:
	PHYSFS_setWriteDir(userdir.c_str());
	PHYSFS_mkdir(dirname.c_str());

	PHYSFS_setWriteDir(oadir.c_str()); //The physfs writedir will now always be the ~/.openanno dir
	if (PHYSFS_addToSearchPath(oadir.c_str(), 1))
		std::cout << "Successfully added " << oadir << " to the physfs search path" << std::endl;

	if (!PHYSFS_exists("config.xml"))
		Config::load("xml/config.xml");
	else
		Config::load("config.xml");

    initScriptBinding();
    ResourceManager::getInstance()->addResourceLoader(new GenericResourceLoader<Script, Resource::SCRIPT>());

    bool foundationLoaded;
    foundationLoaded = ResourceManager::getInstance()->getStaticDelete("scripts/foundation/core.lua", Resource::SCRIPT);
    foundationLoaded = ResourceManager::getInstance()->getStaticDelete("scripts/foundation/input.lua", Resource::SCRIPT) && foundationLoaded;
    if (foundationLoaded) {
        std::cout << "Lua foundation scripts successfully loaded" << std::endl;
    }

    ResourceManager::getInstance()->getStaticDelete("scripts/main.lua", Resource::SCRIPT);

	int i;
	// initialize fTimeFactor calculation
	for (i=4; i >= 0; --i)
	{
		AverageFrameDuration[i] = 1000.0f/fps_limit;
	}

	if(SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE) < 0) {
		std::cout << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
		keepRunning = false;
		return;
	}

#ifndef __arm__
	VideoSystem* screen;
	if(Config::getString("renderer") == "sdl")
	{
		screen = new SDLVideoSystem();
	}
	else
	{
		screen = new OpenGLVideoSystem();
	}
#else
	VideoSystem* screen = new SDLVideoSystem();
#endif

	VideoSystem::initInstance(screen);
	if(!screen->init(Config::getInt("resolution_x"), Config::getInt("resolution_y"), Config::getInt("depth"), Config::getInt("fullscreen")>0))
	{
		keepRunning = false;
		return;
	}

	//show loadscreen:
	SurfacePtr loadscreen = VideoSystem::getInstance()->createSurface("images/misc/splash.png");
	float loadscreenScale = Config::getInt("resolution_x") / static_cast<float>(loadscreen->getWidth());
	screen->drawSurfaceScaled(loadscreen, 0, 0, loadscreenScale);
	VideoSystem::getInstance()->update();

	pAudioSystem = new AudioSystem();
	AudioSystem::initInstance(pAudioSystem);
	pAudioSystem->init(Config::getInt("freq"),
			   true, Config::getInt("channels"),
			   Config::getInt("samples"),
			   16);
	pAudioSystem->setGlobalMusicVolume(Config::getFloat("musicvol"));
	pAudioSystem->setGlobalSoundVolume(Config::getFloat("soundvol"));

	SDL_EnableUNICODE(1);

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	#if defined(PACKAGE_NAME) && defined(PACKAGE_VERSION)
		SDL_WM_SetCaption(PACKAGE_NAME " " PACKAGE_VERSION, NULL);
	#else
		SDL_WM_SetCaption("Openanno", NULL);
	#endif

	startscreen = new StartScreen(0, 0, this);
	pushGameState(startscreen);

	if (!Config::getInt("sound"))
		pAudioSystem->deactivate();
	m_jukebox = new Jukebox("sound/musik/");

	fps_limit = Config::getInt("maxfps");

	ResourceManager::getInstance()->addResourceLoader(new GenericResourceLoader<TileMetaData, Resource::TILEMETADATA>());
}

OpenAnno::~OpenAnno()
{
	GameState* gs;
	while(gs = popGameState())
	{
		delete gs;
		gs = NULL;
	}
	delete m_jukebox;
	VideoSystem::freeInstance();
	AudioSystem::freeInstance();
	PHYSFS_deinit();
	TTF_Quit();
	SDL_Quit();
}

int OpenAnno::run()
{
	unsigned char AverageFrameDurationIndex=0;
	int i;

    // call the lua init function
	ScriptingManager::getInstance()->getScriptSystem()->doString("init()");

	while(keepRunning)
	{
		assert(m_gameStateStack.empty() == 0);
		FrameStartingTime = SDL_GetTicks();
		handleSDLEvents();

		m_gameStateStack.top()->update(fTimeFactor);

		if (Config::getInt("sound"))
			m_jukebox->update();

		// script update function: update(dt)
		MethodArguments updateArguments;
		updateArguments.arg[0].f = fTimeFactor;
		updateArguments.arg[0].m_type = Value::Float;
		ScriptingManager::getInstance()->getScriptSystem()->invokeScriptMethod("update", &updateArguments, 1, 0);


		/* limit framerate to FPS_LIMIT (default 60) to save CPU time
		also calculate a factor (fTimeFactor) to synchronize time depending actions. Simply multiply
		every action with fTimeFactor. For example:
			Position.x += Speed.x*fTimeFactor; */
		FrameDuration = SDL_GetTicks() - FrameStartingTime;

		if ( fps_limit != 0 && (FrameDuration) < (1000/fps_limit) )
		{
			SDL_Delay((1000/fps_limit)-FrameDuration);
			AverageFrameDuration[AverageFrameDurationIndex] = (1000.0f/fps_limit);
		} else
		{
			AverageFrameDuration[AverageFrameDurationIndex] = (float)FrameDuration;
		}

		AverageFrameDurationIndex++;
		if (AverageFrameDurationIndex > 4)
		{
			AverageFrameDurationIndex = 0;
		}

		fTimeFactor = 0.0f;
		for (i=4; i >= 0; --i)
		{
			fTimeFactor += AverageFrameDuration[i];
		}

		fTimeFactor /= 5000; // 5 frame durations plus convert to seconds
		//std::cout << "FPS: " << 1.0f/fTimeFactor << std::endl;

		WidgetManager::getInstance()->updateFrametime(fTimeFactor);
	}

	return 0;
}

const char* OpenAnno::getClassName() const
{
    return "OpenAnno";
}

void OpenAnno::requestExit()
{
	keepRunning = !m_gameStateStack.top()->requestExit();
/*	while ()
	{
		if (m_gameStateStack.size() > 1)
		{
			delete popGameState();
		}
		else
		{
			keepRunning = false;
			break;
		}
	}*/
}

void OpenAnno::exit()
{
	keepRunning = false;
}

void OpenAnno::handleSDLEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
            case SDL_KEYUP:
			{
				WidgetManager::getInstance()->handleSDLInput(event);
				invokeLuaKeyEvent(event.key.keysym.sym, false);
			}
            break;
			case SDL_KEYDOWN:
			{
				WidgetManager::getInstance()->handleSDLInput(event);
				invokeLuaKeyEvent(event.key.keysym.sym, true);
			}
			break;
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
			{
				WidgetManager::getInstance()->handleSDLInput(event);
			}
			break;
			case SDL_QUIT:
			{
				requestExit();
			}
			break;
		}
	}
}

void OpenAnno::invokeLuaKeyEvent(int key, bool isDown)
{
    MethodArguments methodArguments;
    methodArguments.arg[0].f=(float)key;
    methodArguments.arg[0].m_type=Value::Float;
    methodArguments.arg[1].b=isDown;
    methodArguments.arg[1].m_type=Value::Bool;
    ScriptingManager::getInstance()->getScriptSystem()->invokeScriptMethod("keyEvent", &methodArguments, 2, 0);
}


void OpenAnno::pushGameState(GameState* gamestate)
{
	if(m_gameStateStack.size())
	{
		m_gameStateStack.top()->deactivate();
	}

	m_gameStateStack.push(gamestate);
	gamestate->activate();
}

GameState* OpenAnno::popGameState()
{
	if(!m_gameStateStack.size())
	{
		return NULL;
	}

	GameState* result = m_gameStateStack.top();
	m_gameStateStack.pop();
	result->deactivate();

	if(m_gameStateStack.size())
	{
		m_gameStateStack.top()->activate();
	}
	return result;
}
