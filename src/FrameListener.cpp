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

#include "FrameListener.h"
#include "InputManager.h"
#include "Physics.h"
#include "PhysicsDebugDrawer.h"
#include "Character/CharacterController.h"

using namespace QL;


QL::FrameListener::FrameListener(InputManager * inputManager, Physics * physics,
									PhysicsDebugDrawer * physicsDebugDrawer, CharacterController * cc) :
	m_inputManager(inputManager),
	m_physics(physics),
	m_physicsDebugDrawer(physicsDebugDrawer),
	m_characterController(cc)
{
	// m_physics->getDynamicsWorld()->stepSimulation(60 / 100);
}

bool QL::FrameListener::frameRenderingQueued(const Ogre::FrameEvent & evt)
{
	if (m_inputManager->willShutdown())
		return false;
	
	return true;
}

bool QL::FrameListener::frameStarted(const Ogre::FrameEvent & evt)
{
	m_inputManager->captureDevices();

	m_physics->getDynamicsWorld()->stepSimulation(evt.timeSinceLastFrame);
	m_physicsDebugDrawer->step();

	m_characterController->update(m_physics->getDynamicsWorld(), evt.timeSinceLastFrame);

	return true;
}
