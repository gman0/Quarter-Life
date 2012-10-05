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
#include <cstring>
#include <getopt.h>
#include "opts.h"
#include "Convert.h"
// #include "flags.h"

using namespace Ogre;
using namespace std;

const char * getOutputFileName(const char * in)
{
	static char out[100];
	const char * extensionSeparator = strrchr(in, '.');

	if (strcmp(extensionSeparator, ".mesh"))
		sprintf(out, "%s.bullet", in);
	else
		sprintf(out, "%s.bullet", strncat(out, in, strlen(in) - 5));

	return out;
}

int main(int argc, char * argv[])
{
	if (argc < 2)
		printHelp(argv[0]);

	option opts[] = {
		{"help",		no_argument,		0,	'h'},
		{"version",		no_argument,		0,	'v'},
		{"shape-type",	required_argument,	0,	't'},
		{0,				0,				0,	  0}
	};

	int c;
	unsigned flags = 1;

	while ((c = getopt_long(argc, argv, "hvt:", opts, 0)) != -1)
	{
		switch (c)
		{
			case 'h': printHelp(argv[0]); break;
			case 'v': printVersion(); break;
			case 't': getType(optarg, &flags); break;
		}
	}

	Convert * converter = new Convert;

	try
	{
		for (int i = optind; i < argc; i++)
		{
			// sprintf(name, "%s.bullet", argv[i]);
			converter->convertMeshToBullet(argv[i], getOutputFileName(argv[i]), flags);
			// cout << getOutputFileName(argv[i]) << endl;
		}
	}
	catch (Exception & e)
	{
		cerr << "An exception has occured: " << e.getFullDescription().c_str() << endl;
	}

	delete converter;

	return 0;
}
