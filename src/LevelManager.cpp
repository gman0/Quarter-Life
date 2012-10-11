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


#include <boost/filesystem.hpp>
#include "LevelManager.h"
#include "ResourceManager.h"
#include "helpers/ConvertVector3.h"
#include "Renderer.h"

#include <bullet/LinearMath/btVector3.h>

using namespace QL;
using namespace Ogre;

LevelManager::LevelManager(ResourceManager * resourceManager, Renderer * renderer, Physics * physics,
							ScriptManager * scriptManager, GameState * gameState, InputManager * inputManager,
							GuiManager * guiManager) :
	m_physics(physics),
	m_gameState(gameState),
	m_guiManager(guiManager),
	m_resourceManager(resourceManager)
{
	m_triggerManager = new TriggerManager(m_physics);
	m_objectManager = new ObjectManager(m_physics, m_triggerManager, scriptManager);
	m_sceneLoader = new SceneLoader(m_objectManager, m_triggerManager);

	m_loadingState = new LoadingState(m_guiManager, inputManager);

	btVector3 origin(0, 0, 0);
	m_characterController = new CharacterController(renderer->getSceneManager(), renderer->getCamera(),
													inputManager, m_physics, origin);

}

LevelManager::~LevelManager()
{
	delete m_characterController;
	delete m_sceneLoader;
	delete m_objectManager;
	delete m_triggerManager;
}

void LevelManager::loadLevel(const char * levelName, SceneManager * sceneManager)
{
	// m_gameState->addState(m_loadingState);

	using boost::filesystem::path;

	m_levelName = levelName;
	// cleanUpLevel(m_levelName);

	m_objectManager->setSceneManager(sceneManager);

	// Generate the path for the resources so we know where to look for them.
	path base = "media";
	base /= "levels";
	base /= levelName;

	path objects = base / "objects";

	Ogre::ResourceGroupManager * resManager = Ogre::ResourceGroupManager::getSingletonPtr();
	resManager->addResourceLocation(base.c_str(), "FileSystem", levelName);
	resManager->addResourceLocation(objects.c_str(), "ObjFileSystem", levelName);
	resManager->initialiseResourceGroup(levelName);

	String sceneName = levelName;
	sceneName += ".scene";

	m_sceneLoader->parseScene(sceneManager, sceneName, levelName);

	// Warp the character to the starting position.
	// m_characterController->getCharacterPhysics()->warp(convert(m_sceneLoader->getStart()));

	// m_gameState->removeState();

	// PlayState * ps = new PlayState(m_input, m_characterController->getCharacterInput(), m_renderer);
	// m_gameState->addState(ps);
}

void LevelManager::cleanUpLevel(const char * levelName)
{
	ResourceGroupManager::getSingleton().unloadResourceGroup(levelName);
	m_physics->cleanUpWorld();
}

TriggerManager * LevelManager::getTriggerManager() const
{
	return m_triggerManager;
}

ObjectManager * LevelManager::getObjectManager() const
{
	return m_objectManager;
}

SceneLoader * LevelManager::getSceneLoader() const
{
	return m_sceneLoader;
}

CharacterController * LevelManager::getCharacterController() const
{
	return m_characterController;
}
