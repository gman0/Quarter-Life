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

#ifndef QL_FRAME_LISTENER_H
#define QL_FRAME_LISTENER_H

#include <OGRE/OgreFrameListener.h>


class PhysicsDebugDrawer;

namespace QL
{

class InputManager;
class Physics;
class CharacterController;


class FrameListener : public Ogre::FrameListener
{
private:
	InputManager * m_inputManager;

	Physics * m_physics;
	PhysicsDebugDrawer * m_physicsDebugDrawer;

	CharacterController * m_characterController;

public:
	FrameListener(InputManager * inputManager, Physics * physics, PhysicsDebugDrawer * physicsDebugDrawer,
					CharacterController * cc);

	bool frameRenderingQueued(const Ogre::FrameEvent & evt);
	bool frameStarted(const Ogre::FrameEvent & evt);
};


} // namespace QL



#endif // QL_FRAME_LISTENER
