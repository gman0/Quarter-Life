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

#ifndef QL_OBJECT_FILE_ARCHIVE_FACTORY_H
#define QL_OBJECT_FILE_ARCHIVE_FACTORY_H

#include <OGRE/OgreArchiveFactory.h>

namespace QL
{

// The same as Ogre's ZipArchiveFactory but returns a different type name
class ObjectFileArchiveFactory : public Ogre::ArchiveFactory
{
public:
	virtual ~ObjectFileArchiveFactory() {}

	virtual const Ogre::String & getType() const;
	virtual Ogre::Archive * createInstance(const Ogre::String & name);
	virtual void destroyInstance(Ogre::Archive * arch);
};

}


#endif // QL_OBJECT_FILE_ARCHIVE_FACTORY_H
