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


#ifndef QL_CHARACTER_CONTROLLER
#define QL_CHARACTER_CONTROLLER

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreCamera.h>
#include <bullet/LinearMath/btVector3.h>
#include "Character/CharacterPhysics.h"
#include "Character/CharacterInput.h"

class btCollisionWorld;

namespace QL
{

class Physics;
class InputManager;


class CharacterController
{
private:
	CharacterPhysics * m_characterPhysics;
	CharacterInput * m_characterInput;

	/* CharacterStats */

public:
	CharacterController(Ogre::SceneManager * sceneManager, Ogre::Camera * camera, InputManager * inputManger,
						Physics * physics, const btVector3 & origin);
	~CharacterController();

	void update(btCollisionWorld * collisionWorld, btScalar dt);

	CharacterPhysics * getCharacterPhysics();
	CharacterInput * getCharacterInput();
};

} // namespace QL


#endif // QL_CHARACTER_CONTROLLER
