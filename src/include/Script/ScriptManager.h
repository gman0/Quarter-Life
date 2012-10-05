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

#ifndef QL_SCRIPT_MANAGER_H
#define QL_SCRIPT_MANAGER_H

#include <lua.hpp>
#include <bullet/LinearMath/btAlignedObjectArray.h>
#include <OGRE/OgreResourceGroupManager.h>
#include <OGRE/OgreDataStream.h>
#include "Script/Script.h"
#include "Object.h"

namespace QL
{

typedef btAlignedObjectArray<Script*> ScriptArray;

class ScriptManager
{
private:
	lua_State * m_luaState;
	ScriptArray m_scripts;

public:
	ScriptManager();
	~ScriptManager();

	Script * createScript(Ogre::DataStreamPtr & dataStreamPtr);
	lua_State * getLuaState() const;
};

}

#endif // QL_SCRIPT_MANAGER_H
