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


#include "Character/CharacterInput.h"
#include "Character/CharacterController.h"
#include "helpers/ConvertVector3.h"

using namespace QL;
using namespace Ogre;

CharacterInput::CharacterInput(CharacterController * characterController, Ogre::Camera * camera) :
	m_deltaTime(0), m_cameraPitch(0), m_character(characterController), m_camera(camera)
{
	m_keyDirection = Vector3::ZERO;
}

void CharacterInput::update(Ogre::Real dt)
{
	m_deltaTime = dt;
	m_camera->setPosition(convert(m_character->getCharacterPhysics()->getPosition()) + Vector3(0, 3, 0) );

	updateWalkDirection();
}

void CharacterInput::updateWalkDirection()
{
	if (m_character->getCharacterPhysics()->onGround())
	{
		Vector3 goal(0, 0, 0);
		goal += m_keyDirection.z * m_camera->getOrientation().zAxis();
		goal += m_keyDirection.x * m_camera->getOrientation().xAxis();
		goal.y = 0;
		goal.normalise();

		btVector3 walkDirection = convert(goal);

		m_character->getCharacterPhysics()->setWalkDirection(walkDirection);
	}
}

bool CharacterInput::keyPressedCallback(const OIS::KeyEvent & evt)
{
	switch (evt.key)
	{
		case OIS::KC_W:
			m_keyDirection.z -= 1;
			break;
		case OIS::KC_S:
			m_keyDirection.z += 1;
			break;
		case OIS::KC_A:
			m_keyDirection.x -= 1;
			break;
		case OIS::KC_D:
			m_keyDirection.x += 1;
			break;
		case OIS::KC_SPACE:
			m_character->getCharacterPhysics()->jump();
			break;
		case OIS::KC_LCONTROL:
			m_character->getCharacterPhysics()->crouch();
			break;
		default:
			break;
	}

	return true;
}

bool CharacterInput::keyReleasedCallback(const OIS::KeyEvent & evt)
{
	switch (evt.key)
	{
		case OIS::KC_W:
			m_keyDirection.z += 1;
			break;
		case OIS::KC_S:
			m_keyDirection.z -= 1;
			break;
		case OIS::KC_A:
			m_keyDirection.x += 1;
			break;
		case OIS::KC_D:
			m_keyDirection.x += -1;
			break;
		case OIS::KC_LCONTROL:
			m_character->getCharacterPhysics()->crouchEnd();
			break;
		default:
			break;
	}

	return true;
}

bool CharacterInput::mouseMovedCallback(const OIS::MouseEvent & evt)
{
	btScalar sensitivity = 0.007;
	Real rotX = evt.state.X.rel * sensitivity * -1;
	Real rotY = evt.state.Y.rel * sensitivity * -1;

	m_camera->yaw(Radian(rotX));

	/*
	 * This bounds pitch of the camera so that player can't bend his/her
	 * neck like a giraffe...that is if giraffes could do such a thing.
	 * 1.51rad = 90 degrees
	 */
	if (!(m_cameraPitch + rotY > 1.51 && rotY > 0) &&
		!(m_cameraPitch + rotY < -1.51 && rotY < 0))
	{
		m_camera->pitch(Radian(rotY));
		m_cameraPitch += rotY;
	}

	btTransform & transform = m_character->getCharacterPhysics()->getGhostObject()->getWorldTransform();

	btMatrix3x3 orientation = transform.getBasis();
	orientation *= btMatrix3x3(btQuaternion(btVector3(0, 1, 0), rotX));
	transform.setBasis(orientation);

	return true;
}

bool CharacterInput::mousePressedCallback(const OIS::MouseEvent & evt, OIS::MouseButtonID id)
{
	return true;
}

bool CharacterInput::mouseReleasedCallback(const OIS::MouseEvent & evt, OIS::MouseButtonID id)
{
	return true;
}

void CharacterInput::readConfigFile()
{
	ConfigFile cf;
	cf.load("bindings.cfg");

	ConfigFile::SectionIterator sectionIter = cf.getSectionIterator();
	String device, bindName, bindKey;
	ConfigFile::SettingsMultiMap * settings;
	ConfigFile::SettingsMultiMap::iterator i;

	while (sectionIter.hasMoreElements())
	{
		device = sectionIter.peekNextKey();
		settings = sectionIter.getNext();

		for (i = settings->begin(); i != settings->end(); ++i)
		{
			bindName = i->first;
			bindKey = i->second;

			parseBinding(device, bindName, bindKey);
		}
	}
}

void CharacterInput::parseBinding(const String & device, const String & bindName, const String & bindKey)
{

}
