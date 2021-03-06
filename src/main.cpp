/*
    Copyright (C) 2012 Róbert "gman" Vašek <gman@codefreax.org>

    This file is part of Quarter-Life.

    Quarter-Life is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Quarter-Life is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Quarter-Life.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <OGRE/Ogre.h>
#include "Engine.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <windows.h>
#else
#include <iostream>
#endif


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char * argv[])
#endif
{
	QL::Engine engine;

	// try
	{
		engine.run();
	}
/*	catch (Ogre::Exception & e)
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox(0, e.getFullDescription().c_str(), "An exception has occured!",
			MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		std::cerr << "An exception has occured: " << e.getFullDescription().c_str()
					<< std::endl;
#endif
	}
*/
	return 0;
}

