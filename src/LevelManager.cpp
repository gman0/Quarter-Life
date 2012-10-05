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


#include "LevelManager.h"
#include "States/LoadingState.h"
#include "ResourceManager.h"

using namespace QL;
using namespace Ogre;

LevelManager::LevelManager(Physics * physics, ScriptManager * scriptManager, GameState * gameState,
							GuiManager * guiManager, CharacterController * characterController) :
	m_physics(physics),
	m_scriptManager(scriptManager),
	m_gameState(gameState),
	m_guiManager(guiManager),
	m_characterController(characterController)
{
	m_triggerManager = new TriggerManager(m_physics);
	m_objectManager = new ObjectManager(m_physics, m_triggerManager, m_scriptManager);
	m_sceneLoader = new SceneLoader(m_objectManager, m_triggerManager);
}

LevelManager::~LevelManager()
{
	delete m_sceneLoader;
	delete m_objectManager;
	delete m_triggerManager;
}

void LevelManager::loadLevel(const char * levelName, SceneManager * sceneManager)
{
	m_objectManager->setSceneManager(sceneManager);

	// cleanUpLevel();

	// String mapName = levelName;
	// mapName += 
}
