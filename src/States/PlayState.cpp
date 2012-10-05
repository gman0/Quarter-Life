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


#include "States/PlayState.h"
#include "Character/CharacterInput.h"
#include "InputManager.h"
#include "Renderer.h"

using namespace QL;
using namespace Ogre;

PlayState::PlayState(InputManager * inputManager, CharacterInput * input, Renderer * renderer) :
	m_inputManager(inputManager), m_input(input), m_renderer(renderer)
{
	m_timer = m_renderer->getRoot()->getTimer();
}

void PlayState::enter()
{
	// Register input callbacks.
	m_inputManager->registerCallback(m_input);

	// Set GUI, obviously.
	setGUI();
}

void PlayState::update(Ogre::Real dt)
{
	updateFPS();
}

void PlayState::updateFPS()
{
	unsigned long currentTime = m_timer->getMilliseconds();

	if (currentTime - m_lastStatUpdate > 250)
	{
		Ogre::RenderTarget::FrameStats stats = m_renderer->getRenderWindow()->getStatistics();
		m_lastStatUpdate = currentTime;
		String fps = "FPS: ";
		fps += StringConverter::toString((int)stats.lastFPS);

		CEGUI::Window * fpsWindow = m_rootWindow->getChild("Root/fps");
		fpsWindow->setText(fps);
	}
}

void PlayState::exit()
{
}

void PlayState::setGUI()
{
	m_rootWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout("fps.layout");
	CEGUI::System::getSingleton().setGUISheet(m_rootWindow);
}
