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

#ifndef QL_GAME_STATE_H
#define QL_GAME_STATE_H

#include <stack>
#include <OGRE/OgreRoot.h>
#include "State.h"

namespace QL
{

class GameState : public Ogre::FrameListener
{
private:
	std::stack<State*> m_stateStack;

public:
	GameState(Ogre::Root * root);

	virtual bool frameRenderingQueued(const Ogre::FrameEvent & evt);

	void addState(State * st);
	void removeState();
};

} // namespace QL

#endif // QL_GAME_STATE_H
