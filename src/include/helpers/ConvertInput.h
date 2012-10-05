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


#ifndef QL_HELPER_CONVERT_INPUT_H
#define QL_HELPER_CONVERT_INPUT_H


#include <OIS/OISKeyboard.h>

static OIS::KeyCode convert(const char c)
{
	OIS::KeyCode key = (OIS::KeyCode)0x00;

	switch (c)
	{
		case 'a':
		case 'A':
			key = OIS::KC_A;
			break;
		case 'b':
		case 'B':
			key = OIS::KC_B;
			break;
		case 'c':
		case 'C':
			key = OIS::KC_C;
			break;
		case 'd':
		case 'D':
			key = OIS::KC_D;
			break;
		case 'e':
		case 'E':
			key = OIS::KC_E;
			break;
		case 'f':
		case 'F':
			key = OIS::KC_F;
			break;
		case 'g':
		case 'G':
			key = OIS::KC_G;
			break;
		case 'h':
		case 'H':
			key = OIS::KC_H;
			break;
		case 'i':
		case 'I':
			key = OIS::KC_I;
			break;
		case 'j':
		case 'J':
			key = OIS::KC_J;
			break;
		case 'k':
		case 'K':
			key = OIS::KC_K;
			break;
		case 'l':
		case 'L':
			key = OIS::KC_L;
			break;
		case 'm':
		case 'M':
			key = OIS::KC_M;
			break;
		case 'n':
		case 'N':
			key = OIS::KC_N;
			break;
		case 'o':
		case 'O':
			key = OIS::KC_O;
			break;
		case 'p':
		case 'P':
			key = OIS::KC_P;
			break;
		case 'q':
		case 'Q':
			key = OIS::KC_Q;
			break;
		case 'r':
		case 'R':
			key = OIS::KC_R;
			break;
		case 's':
		case 'S':
			key = OIS::KC_S;
			break;
		case 't':
		case 'T':
			key = OIS::KC_T;
			break;
		case 'u':
		case 'U':
			key = OIS::KC_U;
			break;
		case 'v':
		case 'V':
			key = OIS::KC_V;
			break;
		case 'w':
		case 'W':
			key = OIS::KC_W;
			break;
		case 'x':
		case 'X':
			key = OIS::KC_X;
			break;
		case 'y':
		case 'Y':
			key = OIS::KC_Y;
			break;
		case 'z':
		case 'Z':
			key = OIS::KC_Z;
			break;
	}

	return key;
}


#endif // QL_HELPER_CONVERT_INPUT_H
