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
#include "Script/ScriptManager.h"

using namespace QL;
using namespace Ogre;

ScriptManager::ScriptManager()
{
	// m_luaState = lua_open();
}

ScriptManager::~ScriptManager()
{
	for (int i = m_scripts.size() - 1; i >= 0; i--)
		delete m_scripts[i];

	// lua_close(m_luaState);
}

Script * ScriptManager::createScript(DataStreamPtr & dataStreamPtr)
{
	Script * script = new Script(dataStreamPtr);
	m_scripts.push_back(script);

	return script;
	// return 0;
}

lua_State * ScriptManager::getLuaState() const
{
	return m_luaState;
}
