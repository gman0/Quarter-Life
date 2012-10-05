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

#ifndef QL_HELPER_CONVERT_QUATERION_H
#define QL_HELPER_CONVERT_QUATERION_H

#include <OGRE/OgreQuaternion.h>
#include <bullet/LinearMath/btQuaternion.h>

// Helper functions for converting quaternions from Ogre to Bullet and vice versa.

static Ogre::Quaternion convert(const btQuaternion & q)
{
	return Ogre::Quaternion(q.w(), q.x(), q.y(), q.z());
}

static btQuaternion convert(const Ogre::Quaternion & q)
{
	return btQuaternion(q.x, q.y, q.z, q.w);
}

#endif // QL_HELPER_CONVERT_QUATERINON_H
