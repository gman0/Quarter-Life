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

#include <luabind/luabind.hpp>
#include <cstdio>
#include "Script/Script.h"

using namespace QL;
using namespace Ogre;

Script::Script(DataStreamPtr & dataStreamPtr)
{
	// /*
	m_bufSize = dataStreamPtr->size();
	m_scriptBuffer = new char[m_bufSize];

	dataStreamPtr->read(m_scriptBuffer, m_bufSize);

	m_luaState = luaL_newstate();

	// Parse the Lua script.
	luaL_dostring(m_luaState, m_scriptBuffer);

	// Load Lua's std libs.
	luaL_openlibs(m_luaState);

	luabind::open(m_luaState);
	// */
}

lua_State * Script::getLuaState() const
{
	return m_luaState;
}

Script::~Script()
{
	lua_close(m_luaState);
	delete [] m_scriptBuffer;
}
