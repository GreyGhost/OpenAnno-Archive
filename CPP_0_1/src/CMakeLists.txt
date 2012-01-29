set(SDL_gfx_DIR ${CMAKE_SOURCE_DIR})

add_subdirectory(lib)

add_definitions(-DTIXML_USE_STL)
include_directories(${CMAKE_SOURCE_DIR}/src/lib/lua ${CMAKE_SOURCE_DIR}/src)

if(MINGW)
	ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/openanno.o
	COMMAND windres -i ${CMAKE_SOURCE_DIR}/src/openanno.rc --include-dir ${CMAKE_SOURCE_DIR}/src -o ${CMAKE_CURRENT_BINARY_DIR}/openanno.o
	DEPENDS ${CMAKE_SOURCE_DIR}/src/openanno.rc)

	SET(oaRCFiles ${CMAKE_CURRENT_BINARY_DIR}/openanno.o)
endif(MINGW)

set(oaSrcs configuration.cpp entity.cpp entityManager.cpp gamestate.cpp grid.cpp jukebox.cpp openanno.cpp audio/audioSystem.cpp audio/music.cpp audio/sample.cpp gui/checkbox.cpp gui/commandwidget.cpp gui/configdialog.cpp gui/dialog.cpp gui/editbox.cpp gui/imagebutton.cpp gui/logindialog.cpp gui/menu.cpp gui/multidialog.cpp gui/newgamedialog.cpp gui/registerdialog.cpp gui/scrollbar.cpp gui/searchgames.cpp gui/selectbox.cpp gui/selector.cpp gui/singledialog.cpp gui/slider.cpp gui/startscreen.cpp gui/statictext.cpp gui/table.cpp gui/teamdialog.cpp gui/textviewer.cpp gui/widget.cpp gui/widgetmanager.cpp io/resource.cpp io/resourceManager.cpp scripting/class.cpp scripting/luaScriptSystem.cpp scripting/method.cpp scripting/script.cpp scripting/scriptingManager.cpp sprite/spriteanimation.cpp sprite/sprite.cpp video/color.cpp video/point.cpp video/videosystem.cpp video/opengl/openglfont.cpp video/opengl/openglsurface.cpp video/opengl/openglvideosystem.cpp video/opengl/texture.cpp video/sdl/sdlfont.cpp video/sdl/sdlsurface.cpp video/sdl/sdlvideosystem.cpp gui/events/eventcommand.cpp gui/events/event.cpp gui/events/eventkey.cpp gui/events/eventmouseclick.cpp gui/events/eventmousemove.cpp gui/events/eventmouseout.cpp gui/events/eventmouseover.cpp gui/events/eventtick.cpp ingame/ingame.cpp ingame/map.cpp ingame/minimap.cpp ingame/tile.cpp ingame/island.cpp ingame/mission.cpp)


add_executable(openanno ${oaSrcs} ${oaRCFiles})

find_package(PhysFS REQUIRED)
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_gfx REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${PHYSFS_INCLUDE_DIR} ${SDL_INCLUDE_DIR}
${SDLIMAGE_INCLUDE_DIR} ${SDLMIXER_INCLUDE_DIR} ${SDLTTF_INCLUDE_DIR}
${SDLGFX_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
target_link_libraries(openanno lua tinyxml ${PHYSFS_LIBRARY} ${SDL_LIBRARY}
${SDLIMAGE_LIBRARY} ${SDLMIXER_LIBRARY} ${SDLTTF_LIBRARY} ${SDLGFX_LIBRARY} ${OPENGL_LIBRARIES})

install(TARGETS openanno DESTINATION bin)
