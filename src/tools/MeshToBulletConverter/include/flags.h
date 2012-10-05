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

#ifndef FLAGS_H
#define FLAGS_H

/*
 * These flags are used to specify how should be
 * resulting collision shape created.
 *
 * S_*  - shape
 * T_*  - type static - input mesh doesn't contain animations,
 * 			   dynamic - input mesh contains animations
 */
enum FLAGS
{
	S_TRIMESH	= 1 << 0,
	S_SPHERE	= 1 << 1,
	S_BOX		= 1 << 2,
	S_CYLINDER	= 1 << 3,
	S_CONVEX	= 1 << 4,

	T_STATIC	= 1 << 5,
	T_DYNAMIC	= 1 << 6
};

#endif // FLAGS_H
