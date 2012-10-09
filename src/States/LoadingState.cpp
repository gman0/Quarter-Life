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

#include "States/LoadingState.h"
#include "GuiManager.h"
#include "InputManager.h"

#include <unistd.h>

using namespace QL;

LoadingState::LoadingState(GuiManager * guiManager, InputManager * inputManager) :
	m_guiManager(guiManager), m_inputManager(inputManager)
{}

void LoadingState::enter()
{
	m_inputManager->registerCallback(&m_inputCallback);
	m_guiManager->setLayout("loading.layout");
	// sleep(10);
}

void LoadingState::exit()
{}
