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


#ifndef QL_RENDERER_H
#define QL_RENDERER_H

#include <OGRE/Ogre.h>

class PhysicsDebugDrawer;

namespace QL
{

class FrameListener;
class InputManager;
class Physics;
class CharacterController;
class CharacterCamera;

/*
 * Creates the rendering window and handles rendering.
 */
class Renderer
{
private:
	Ogre::Root * m_root;
	Ogre::RenderWindow * m_window;
	Ogre::Viewport * m_viewport;
	Ogre::SceneManager * m_sceneManager;
	FrameListener * m_frameListener;

	Ogre::Camera * m_camera;

	PhysicsDebugDrawer * m_physicsDebugDrawer;

public:
	Renderer();
	~Renderer();
	
	// Initialise viewport, create our FrameListener and initialise resources.
	void init(InputManager * inputManager, Physics * physics,
				CharacterController * characterController);

	// Start rendering (after calling init()).
	void startRendering() const;


	Ogre::Root * getRoot() const;
	Ogre::RenderWindow * getRenderWindow() const;
	Ogre::Viewport * getViewport() const;
	Ogre::Camera * getCamera() const;
	Ogre::SceneManager * getSceneManager() const;

private:
	void createCamera();
};

} // namespace QL


#endif // QL_RENDERER_H
