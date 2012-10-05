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

#ifndef QL_ENGINE_H
#define QL_ENGINE_H

namespace QL
{


class Renderer;
class ResourceManager;
class Physics;
class InputManager;
class CharacterController;
class GameState;


/*
 * Main class which initialises all layers (rendering, physics etc...)
 * needed to run the game.
 */
class Engine
{
private:

	Renderer * m_renderer;
	ResourceManager * m_resourceManager;
	Physics * m_physics;
	InputManager * m_inputManager;
	CharacterController * m_characterController;
	GameState * m_gameState;
/*	AI * m_ai;
	GUI * m_gui;
	Audio * m_audio;
	SceneManager * m_sceneManager;
*/
public:
	Engine();
	~Engine();
/*
	QL::Renderer *		getRenderer();
	QL::Physics *		getPhysics();
	QL::AI *			getAI();
	QL::GUI *			getGUI();
	QL::Audio *			getAudio();
	QL::SceneManager *	getSceneManager();
	QL::InputManager *	getInputManager();
*/
	//  Enter the infinite loop and start rendering, updating physics etc.
	void run();
};

} // namespace QL

#endif // QL_ENGINE_H
