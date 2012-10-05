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


#ifndef QL_LEVEL_MANAGER_H
#define QL_LEVEL_MANAGER_H

#include <OGRE/OgreSceneManager.h>
#include "Physics.h"
#include "Script/ScriptManager.h"
#include "GameState.h"
#include "GuiManager.h"
#include "Character/CharacterController.h"
#include "TriggerManager.h"
#include "ObjectManager.h"
#include "SceneLoader.h"

namespace QL
{

class LevelManager
{
private:
	ObjectManager * m_objectManager;
	SceneLoader * m_sceneLoader;
	TriggerManager * m_triggerManager;
	Physics * m_physics;
	ScriptManager * m_scriptManager;
	GameState * m_gameState;
	GuiManager * m_guiManager;
	CharacterController * m_characterController;

public:
	LevelManager(Physics * physics, ScriptManager * scriptManager, GameState * gameState,
				GuiManager * guiManager, CharacterController * characterController);
	~LevelManager();

	void loadLevel(const char * levelName, Ogre::SceneManager * sceneManager);

private:
	// Remove all objects from the level.
	void cleanUpLevel();
};

}

#endif // QL_LEVEL_MANAGER_H
