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


#ifndef QL_CHARACTER_INPUT_H
#define QL_CHARACTER_INPUT_H

#include <OIS/OIS.h>
#include <OGRE/Ogre.h>
#include "InputCallbackInterface.h"

namespace QL
{

class CharacterController;

class CharacterInput : public InputCallbackInterface
{
private:
	Ogre::Real m_deltaTime;
	Ogre::Real m_cameraPitch;
	CharacterController * m_character;
	Ogre::Camera * m_camera;

	Ogre::Vector3 m_keyDirection;

public:
	CharacterInput(CharacterController * characterController, Ogre::Camera * camera);

	void update(Ogre::Real dt);

	bool keyPressedCallback(const OIS::KeyEvent & evt);
	bool keyReleasedCallback(const OIS::KeyEvent & evt);
	bool mouseMovedCallback(const OIS::MouseEvent & evt);
	bool mousePressedCallback(const OIS::MouseEvent & evt, OIS::MouseButtonID id);
	bool mouseReleasedCallback(const OIS::MouseEvent & evt, OIS::MouseButtonID id);

private:
	void updateWalkDirection();
	void readConfigFile();
	void parseBinding(const Ogre::String & device, const Ogre::String & bindName,
						const Ogre::String & bindKey);
};


} // namespace QL


#endif // QL_CHARACTER_INPUT_H
