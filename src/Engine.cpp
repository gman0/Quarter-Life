#include <bullet/LinearMath/btVector3.h>
#include "Engine.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Physics.h"
#include "InputManager.h"
#include "Character/CharacterController.h"
#include "GameState.h"
#include "States/PlayState.h"
#include "SceneLoader.h"
#include "ObjectManager.h"
#include "GuiManager.h"
#include "Script/ScriptManager.h"
#include "helpers/ConvertVector3.h"
using namespace QL;
using namespace Ogre;

Engine::Engine()
{
	m_renderer = new Renderer;
	m_resourceManager = new ResourceManager("resources.cfg");
	m_physics = new Physics;
	m_inputManager = new InputManager(m_renderer->getRenderWindow());
	m_gameState = new GameState(m_renderer->getRoot());
	m_characterController = 0;
}

Engine::~Engine()
{
	delete m_characterController;
	delete m_gameState;
	delete m_inputManager;
	delete m_physics;
	delete m_resourceManager;
	delete m_renderer;
}

void Engine::run()
{
	m_resourceManager->initialiseResources();

	SceneManager * sceneManager = m_renderer->getSceneManager();
	ScriptManager * scriptManager = new ScriptManager;
	TriggerManager * triggerManager = new TriggerManager(m_physics);
	ObjectManager objManager(m_physics, triggerManager, scriptManager);
	objManager.setSceneManager(sceneManager);
	SceneLoader * sl = new SceneLoader(&objManager, triggerManager);
	sl->parseScene(m_renderer->getSceneManager(), "asd.scene", "Test");

	btVector3 origin = convert(sl->getStart());
	m_characterController = new CharacterController(m_renderer->getSceneManager(), m_renderer->getCamera(),
													m_inputManager, m_physics, origin);

	m_renderer->init(m_inputManager, m_physics, m_characterController);

	GuiManager * guiManager = new GuiManager(m_renderer);

	PlayState * ps = new PlayState(m_inputManager, m_characterController->getCharacterInput(), m_renderer);
	m_gameState->addState(ps);

	Light * light = sceneManager->createLight("Ambient");
	light->setType(Light::LT_DIRECTIONAL);
	light->setDirection(Vector3(1, -1, 0));

	/*
	SceneNode * tableNode = sceneManager->getRootSceneNode()->createChildSceneNode(Vector3(3, 5, 0));
	tableNode->setScale(Vector3(2.943986, 0.179007, 2.943986));
	ObjectFilePtr objFilePtr = ObjectFileManager::getSingleton().load("table.obj", "Test");
	objManager.addObject(objFilePtr, tableNode, 3);
	*/

	/** TEST */

	m_renderer->startRendering();

	delete ps;
	delete guiManager;
	delete sl;
	delete triggerManager;
	delete scriptManager;
}
