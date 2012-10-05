cmake_minimum_required(VERSION 2.6)
 
project(quarter-life)

if(UNIX)
	set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	set(OGRE_SAMPLES_INCLUDEPATH
		/usr/local/share/OGRE/samples/Common/include/
	)
	set(BULLET_INCLUDEPATH
		/usr/local/include/bullet/
	)
	set(BULLET_LIBRARIES
		BulletWorldImporter
		BulletDynamics
		BulletCollision
		BulletFileLoader
		LinearMath
	)
	set(TINYXML_LIBRARIES
		tinyxml2
	)
	set(LUA_INCLUDEPATH
		/usr/include/lua5.1/
	)
	set(LUA_LIBRARIES
		lua5.1
	)
	set(LUABIND_LIBRARIES
		luabind
	)
	set(CEGUI_LIBRARIES
		CEGUIBase
		CEGUIOgreRenderer
	)
endif(UNIX)
 
if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()
 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

set(CMAKE_DEBUG_POSTFIX "_d")
 
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}")
 
find_package(OGRE REQUIRED)
 

 
find_package(OIS REQUIRED)
 
if(NOT OIS_FOUND)
	message(SEND_ERROR "Failed to find OIS.")
endif()
 
# Find Boost
if (NOT OGRE_BUILD_PLATFORM_IPHONE)
	if (WIN32 OR APPLE)
		set(Boost_USE_STATIC_LIBS TRUE)
	else ()
		# Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
		set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
	endif ()
	if (MINGW)
		# this is probably a bug in CMake: the boost find module tries to look for
		# boost libraries with name libboost_*, but CMake already prefixes library
		# search names with "lib". This is the workaround.
		set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "")
	endif ()
	set(Boost_ADDITIONAL_VERSIONS "1.49.0")

	# Components that need linking (NB does not include header-only components like bind)
	set(OGRE_BOOST_COMPONENTS thread date_time)
	find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
	if (NOT Boost_FOUND)
		# Try again with the other type of libs
		set(Boost_USE_STATIC_LIBS NOT ${Boost_USE_STATIC_LIBS})
		find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
	endif()
	find_package(Boost QUIET)
 
	# Set up referencing of Boost
	include_directories(${Boost_INCLUDE_DIR})
	add_definitions(-DBOOST_ALL_NO_LIB)
	set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${Boost_LIBRARIES})
endif()
 
set(SRCS
	./main.cpp
	./Engine.cpp
	./FrameListener.cpp
	./GameState.cpp
	./InputManager.cpp
	./Physics.cpp
	./PhysicsDebugDrawer.cpp
	./Renderer.cpp
	./ObjectFileArchiveFactory.cpp
	./ObjectFile.cpp
	./ObjectFileManager.cpp
	./ObjectManager.cpp
	./ResourceManager.cpp
	./TriggerManager.cpp
	./Trigger.cpp
	./SceneLoader.cpp
	./GuiManager.cpp
	./LevelManager.cpp
)
 
include_directories(
	"${PROJECT_SOURCE_DIR}/include"
	${BULLET_INCLUDEPATH}
	${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	${OGRE_SAMPLES_INCLUDEPATH}
	${LUA_INCLUDEPATH}
)

add_subdirectory(Character)
add_subdirectory(States)
add_subdirectory(Script)

add_executable(quarter-life ${SRCS}) 
set_target_properties(quarter-life PROPERTIES DEBUG_POSTFIX _d)
target_link_libraries(quarter-life
	Character
	GameStates
	Script
	${BULLET_LIBRARIES}
	${OGRE_LIBRARIES}
	${OIS_LIBRARIES}
	${TINYXML_LIBRARIES}
	${LUA_LIBRARIES}
	${LUABIND_LIBRARIES}
	${CEGUI_LIBRARIES}
)
 
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
 
if(MINGW OR UNIX)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
endif(MINGW OR UNIX)

