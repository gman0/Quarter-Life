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


#ifndef QL_INPUT_CALLBACK_INTERFACE_H
#define QL_INPUT_CALLBACK_INTERFACE_H

#include <OIS/OIS.h>


namespace QL
{

// This is a base class for all handling input injecting into InputManager.
class InputCallbackInterface
{
public:
	virtual ~InputCallbackInterface() {}

	virtual bool keyPressedCallback(const OIS::KeyEvent&) { return true; }
	virtual bool keyReleasedCallback(const OIS::KeyEvent&) { return true; }

	virtual bool mouseMovedCallback(const OIS::MouseEvent&) { return true; }
	virtual bool mousePressedCallback(const OIS::MouseEvent&, OIS::MouseButtonID) { return true; }
	virtual bool mouseReleasedCallback(const OIS::MouseEvent&, OIS::MouseButtonID) { return true; }
};


} // namespace QL


#endif // QL_INPUT_CALLBACK_INTERFACE
