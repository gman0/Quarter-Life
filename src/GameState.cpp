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

#include <cassert>
#include "GameState.h"

using namespace QL;
using namespace std;
using namespace Ogre;

GameState::GameState(Root * root)
{
	root->addFrameListener(this);
}

void GameState::addState(State * st)
{
	if (!m_stateStack.empty())
		m_stateStack.top()->exit();

	m_stateStack.push(st);
	st->enter();
}

void GameState::removeState()
{
	int stackSize = m_stateStack.size();

	assert(stackSize && "Attempt to remove a state from empty m_stateStack");

	State * st = m_stateStack.top();
	st->exit();

	m_stateStack.pop();

	if (stackSize > 1)
		m_stateStack.top()->enter();
}

bool GameState::frameRenderingQueued(const FrameEvent & evt)
{
	if (!m_stateStack.empty())
		m_stateStack.top()->update(evt.timeSinceLastEvent);

	return true;
}
