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

#include "Physics.h"
#include "Character/CharacterController.h"

using namespace QL;

CharacterController::CharacterController(Ogre::SceneManager * sceneManager, Ogre::Camera * camera,
										InputManager * inputManager, Physics * physics, const btVector3 & origin)
{
	m_characterPhysics = new CharacterPhysics(physics);

	m_characterPhysics->warp(origin);

	m_characterInput = new CharacterInput(this, camera);
}

CharacterController::~CharacterController()
{
	delete m_characterInput;
	delete m_characterPhysics;
}

void CharacterController::update(btCollisionWorld * collisionWorld, btScalar dt)
{
	m_characterInput->update(dt);
}

CharacterPhysics * CharacterController::getCharacterPhysics()
{
	return m_characterPhysics;
}

CharacterInput * CharacterController::getCharacterInput()
{
	return m_characterInput;
}
