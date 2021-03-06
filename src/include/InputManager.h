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

#ifndef QL_INPUT_MANAGER_H
#define QL_INPUT_MANAGER_H

#include <OIS/OIS.h>
#include <OGRE/OgreRenderWindow.h>
#include <vector>

namespace QL
{

class InputCallbackInterface;


class InputManager : public OIS::KeyListener, public OIS::MouseListener
{
private:
	OIS::InputManager * m_inputManager;
	OIS::Keyboard * m_keyboard;
	OIS::Mouse * m_mouse;

	InputCallbackInterface * m_inputCallback;

	bool m_shutdown;

public:
	InputManager(Ogre::RenderWindow * window);
	~InputManager();

	virtual bool keyPressed(const OIS::KeyEvent & evt);
	virtual bool keyReleased(const OIS::KeyEvent & evt);

	virtual bool mouseMoved(const OIS::MouseEvent & evt);
	virtual bool mousePressed(const OIS::MouseEvent & evt, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent & evt, OIS::MouseButtonID id);

	void captureDevices();

	/*
	 * This method registers a callback class which is then used
	 * in the virtual methods above.
	 */
	void registerCallback(InputCallbackInterface * callback);

	// TODO: remove this
	bool willShutdown() const;
};

} // namespace QL

#endif // QL_INPUT_MANAGER_H
