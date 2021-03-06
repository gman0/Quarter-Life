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
#include "LevelManager.h"
using namespace QL;
using namespace Ogre;

Engine::Engine()
{
	m_renderer = new Renderer;
	m_resourceManager = new ResourceManager("resources.cfg");
	m_resourceManager->initialiseResources();
	m_physics = new Physics;
	m_inputManager = new InputManager(m_renderer->getRenderWindow());
	m_gameState = new GameState(m_renderer->getRoot());
	m_scriptManager = new ScriptManager;
	m_guiManager = new GuiManager(m_renderer);
	m_levelManager = new LevelManager(m_resourceManager, m_renderer, m_physics, m_scriptManager, m_gameState,
										m_inputManager, m_guiManager);
}

Engine::~Engine()
{
	delete m_levelManager;
	delete m_guiManager;
	delete m_scriptManager;
	delete m_gameState;
	delete m_inputManager;
	delete m_physics;
	delete m_resourceManager;
	delete m_renderer;
}

void Engine::run()
{
	CharacterController * characterController = m_levelManager->getCharacterController();

	m_renderer->init(m_inputManager, m_physics, characterController);

	/** TEST */
	m_levelManager->loadLevel("railstation", m_renderer->getSceneManager());
	PlayState * ps = new PlayState(m_inputManager, characterController->getCharacterInput(), m_renderer);
	m_gameState->addState(ps);
	/** TEST */

	m_renderer->startRendering();

	delete ps;
}
