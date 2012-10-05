cmake_minimum_required(VERSION 2.6)
 
project(MeshToBulletConverter)

if(UNIX)
	set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	set(BULLET_INCLUDEPATH
		/usr/local/include/bullet/
	)
	set(BULLET_LIBRARIES
		BulletDynamics
		BulletCollision
		LinearMath
	)
	message("${PROJECT_BUILD_DIR}")
endif(UNIX)
 
if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()
 
set(CMAKE_DEBUG_POSTFIX "_d")
 
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}")
 
find_package(OGRE REQUIRED)
 

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
	./opts.cpp
	./Convert.cpp
)

include_directories(
	"${PROJECT_SOURCE_DIR}/include" # copy BtOgre's headers to here
	${BULLET_INCLUDEPATH}
	${OGRE_INCLUDE_DIRS}
)

link_directories(
	"${CMAKE_BINARY_DIR}" # place libBtOgre.a here
)

add_executable(MeshToBulletConverter ${SRCS}) 
set_target_properties(MeshToBulletConverter PROPERTIES DEBUG_POSTFIX _d)
target_link_libraries(MeshToBulletConverter
	BtOgre
	${BULLET_LIBRARIES}
	${OGRE_LIBRARIES}
)
 
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
 
if(MINGW OR UNIX)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
endif(MINGW OR UNIX)

