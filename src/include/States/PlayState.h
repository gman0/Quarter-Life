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


#ifndef QL_PLAY_STATE_H
#define QL_PLAY_STATE_H

#include <OGRE/OgreTimer.h>
#include "State.h"
#include "GuiManager.h"

namespace QL
{

class InputManager;
class CharacterInput;
class Renderer;

class PlayState : public State
{
private:
	InputManager * m_inputManager;
	CharacterInput * m_input;
	Renderer * m_renderer;

	// These are for FPS counter.
	Ogre::Timer * m_timer;
	unsigned long m_lastStatUpdate;

	CEGUI::Window * m_rootWindow;

public:
	PlayState(InputManager * inputManager, CharacterInput * input, Renderer * renderer);

	virtual void enter();
	virtual void update(Ogre::Real dt);
	virtual void exit();

private:
	void setGUI();
	void updateFPS();
};


} // namespace QL


#endif // QL_PLAY_STATE_H
