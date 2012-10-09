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


#include "Renderer.h"
#include "GuiManager.h"

using namespace QL;
using namespace CEGUI;

GuiManager::GuiManager(Renderer * renderer) : m_window(0), m_renderer(renderer)
{
	// Instantiate CEGUI's Ogre renderer.
	m_ceguiRenderer = &OgreRenderer::bootstrapSystem();

	// Define resource groups for CEGUI's components.
	Imageset::setDefaultResourceGroup("Imagesets");
	Font::setDefaultResourceGroup("Fonts");
	Scheme::setDefaultResourceGroup("Schemes");
	WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	WindowManager::setDefaultResourceGroup("Layouts");

	// Set default skin.
	SchemeManager::getSingleton().create("TaharezLook.scheme");

	// Set default mouse cursor.
	// System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

}

GuiManager::~GuiManager()
{
	OgreRenderer::destroySystem();
}

Window * GuiManager::setLayout(const char * layoutName)
{
	m_window = WindowManager::getSingleton().loadWindowLayout(layoutName);
	return m_window;
}

Window * GuiManager::getRootWindow() const
{
	return m_window;
}
