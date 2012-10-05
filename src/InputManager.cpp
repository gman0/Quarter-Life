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

#include "InputManager.h"
#include "InputCallbackInterface.h"

using namespace QL;

InputManager::InputManager(Ogre::RenderWindow * window) : m_inputCallback(0)
{
	OIS::ParamList pl;
	unsigned long windowHnd = 0;
	std::ostringstream windowHndStr;

	window->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;

	pl.insert(std::make_pair("WINDOW", windowHndStr.str()));

	// ---
	pl.insert(std::make_pair("x11_keyboard_grab", "false"));
	// pl.insert(std::make_pair("x11_mouse_grab", "false"));
	// pl.insert(std::make_pair("x11_mouse_hide", "false"));
	// ---

	m_inputManager = OIS::InputManager::createInputSystem(pl);
	m_keyboard = static_cast<OIS::Keyboard*>(m_inputManager->createInputObject(OIS::OISKeyboard, true));
	m_mouse = static_cast<OIS::Mouse*>(m_inputManager->createInputObject(OIS::OISMouse, true));

	m_keyboard->setEventCallback(this);
	m_mouse->setEventCallback(this);
}

InputManager::~InputManager()
{
	m_inputManager->destroyInputObject(m_keyboard);
	m_inputManager->destroyInputObject(m_mouse);
	OIS::InputManager::destroyInputSystem(m_inputManager);
}

bool InputManager::keyPressed(const OIS::KeyEvent & evt)
{
	// FIXME: remove this later
	if (evt.key == OIS::KC_ESCAPE)
		m_shutdown = true;

	return m_inputCallback->keyPressedCallback(evt);
}

bool InputManager::keyReleased(const OIS::KeyEvent & evt)
{
	return m_inputCallback->keyReleasedCallback(evt);
}

bool InputManager::mouseMoved(const OIS::MouseEvent & evt)
{
	return m_inputCallback->mouseMovedCallback(evt);
}

bool InputManager::mousePressed(const OIS::MouseEvent & evt, OIS::MouseButtonID id)
{
	return m_inputCallback->mousePressedCallback(evt, id);
}

bool InputManager::mouseReleased(const OIS::MouseEvent & evt, OIS::MouseButtonID id)
{
	return m_inputCallback->mouseReleasedCallback(evt, id);
}

void InputManager::captureDevices()
{
	m_keyboard->capture();
	m_mouse->capture();
}

void InputManager::registerCallback(InputCallbackInterface * callback)
{
	m_inputCallback = callback;
}

bool InputManager::willShutdown() const
{
	return m_shutdown;
}
