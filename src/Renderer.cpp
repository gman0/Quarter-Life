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

#include "Renderer.h"
#include "FrameListener.h"
#include "InputManager.h"
#include "Physics.h"
#include "Character/CharacterController.h"
#include "PhysicsDebugDrawer.h"
#include "ObjectFileManager.h"
#include <cassert>

using namespace QL;
using namespace Ogre;

Renderer::Renderer() :
	m_root(0),
	m_window(0),
	m_viewport(0),
	m_sceneManager(0),
	m_frameListener(0),
	m_physicsDebugDrawer(0)
{
	m_root = new Root;

	// TODO: replace this with the actual game options menu
	assert((m_root->restoreConfig() || m_root->showConfigDialog()) && "Cannot get Ogre configuration");

	m_window = m_root->initialise(true, "Quarter-Life");
	m_sceneManager = m_root->createSceneManager(Ogre::ST_GENERIC);

	createCamera();

	m_viewport = m_window->addViewport(m_camera);
	m_viewport->setBackgroundColour(ColourValue(0, 0, 0));

	m_camera->setAspectRatio(Real(m_viewport->getActualWidth()) / Real(m_viewport->getActualHeight()));
	// m_camera->setPolygonMode(Ogre::PM_POINTS);
}

Renderer::~Renderer()
{
	delete m_physicsDebugDrawer;
	delete m_frameListener;
	delete m_root;
}

void Renderer::init(InputManager * inputManager, Physics * physics, CharacterController * character)
{
	m_physicsDebugDrawer = new PhysicsDebugDrawer(m_sceneManager->getRootSceneNode(),
													physics->getDynamicsWorld());
	// /*
	m_physicsDebugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawAabb |
										btIDebugDraw::DBG_DrawContactPoints);
	// */
	// m_physicsDebugDrawer->setDebugMode(0);
	physics->getDynamicsWorld()->setDebugDrawer(m_physicsDebugDrawer);

	m_frameListener = new FrameListener(inputManager, physics, m_physicsDebugDrawer, character /*...*/);
	m_root->addFrameListener(m_frameListener);

	// Shadow technique
	// m_sceneManager->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
}

void Renderer::startRendering() const
{
	m_root->startRendering();
}

Root * Renderer::getRoot() const
{
	return m_root;
}

RenderWindow * Renderer::getRenderWindow() const
{
	return m_window;
}

Viewport * Renderer::getViewport() const
{
	return m_viewport;
}

Camera * Renderer::getCamera() const
{
	return m_camera;
}

SceneManager * Renderer::getSceneManager() const
{
	return m_sceneManager;
}

void Renderer::createCamera()
{
	m_camera = m_sceneManager->createCamera("CharacterCamera");
	m_camera->setNearClipDistance(1);
	m_camera->setFixedYawAxis(true);
}
