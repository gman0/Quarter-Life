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

#ifndef QL_HELPER_CONVERT_VECTOR3_H
#define QL_HELPER_CONVERT_VECTOR3_H

#include <OGRE/OgreVector3.h>
#include <bullet/LinearMath/btVector3.h>

// Helper functions for converting vectors from Ogre to Bullet and vice versa.

static Ogre::Vector3 convert(const btVector3 & vec)
{
	return Ogre::Vector3(vec.x(), vec.y(), vec.z());
}

static btVector3 convert(const Ogre::Vector3 & vec)
{
	return btVector3(vec.x, vec.y, vec.z);
}

#endif // QL_HELPER_CONVERT_VECTOR3_H
