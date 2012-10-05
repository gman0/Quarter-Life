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

#include <iostream>
#include <cstdlib>
#include <cstring>
#include "flags.h"

#define PACKAGE_NAME "MeshToBulletConverter"
#define VERSION "1.0.0"
#define VERSION_MAX   1
#define VERSION_MIN   0
#define VERSION_PATCH 0

using namespace std;

void printHelp(const char * progName)
{
	cout << PACKAGE_NAME << " converts Ogre meshes to .bullet collision shapes.\n";
	cout << "Usage: " << progName << " [-hv] input.mesh\n"
		 << "\t-h\t--help\t\tPrints this help message.\n"
		 << "\t-v\t--version\tPrints version."
		 << endl;

	exit(1);
}

void printVersion()
{
	cout << PACKAGE_NAME << " " << VERSION << endl;
	exit(1);
}

void getType(const char * type, unsigned * flags)
{
	if (strcmp(type, "trimesh") == 0)
		*flags = S_TRIMESH;
	else if (strcmp(type, "sphere") == 0)
		*flags = S_SPHERE;
	else if (strcmp(type, "box") == 0)
		*flags = S_BOX;
	else if (strcmp(type, "cylinder") == 0)
		*flags = S_CYLINDER;
	else if (strcmp(type, "convex") == 0)
		*flags = S_CONVEX;
	else
	{
		cerr << "Unrecognised collision shape type '" << type << "' in option --shape-type, defaulting to trimesh\n";
		*flags |= S_TRIMESH;
	}
}
