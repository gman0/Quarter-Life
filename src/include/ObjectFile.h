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


#ifndef QL_OBJECT_FILE_H
#define QL_OBJECT_FILE_H

#include <OGRE/OgreResourceManager.h>
#include <OGRE/OgreArchive.h>

namespace QL
{


/*
 * An object file (.obj) is a ZIP archive which normally
 * consits of an Ogre mesh file (.mesh) and a Bullets
 * collision shape (.bullet).
 *
 * If an object has _trigger postfix (example_trigger.obj),
 * an object file is used as a trigger callback. That means
 * the .mesh file is optional, must contain a .bullet file
 * and a python script which is used as a callback when the
 * trigger is triggered.
 */
class ObjectFile : public Ogre::Resource
{
private:
	const Ogre::String & m_name;
	Ogre::Archive * m_archive;
	size_t m_size;

public:
	ObjectFile(Ogre::ResourceManager * creator, const Ogre::String & name, Ogre::ResourceHandle handle,
				const Ogre::String & group, bool isManual = false, Ogre::ManualResourceLoader * loader = 0);
	virtual ~ObjectFile();

	Ogre::Archive * getArchive();

protected:
	virtual void loadImpl();
	virtual void unloadImpl();
	virtual size_t calculateSize() const;

};


class ObjectFilePtr : public Ogre::SharedPtr<ObjectFile>
{
public:
	ObjectFilePtr();
	explicit ObjectFilePtr(ObjectFile * ptr);
	ObjectFilePtr(const ObjectFilePtr & ref);
	ObjectFilePtr(const Ogre::ResourcePtr & resourceRef);

	ObjectFilePtr & operator=(const Ogre::ResourcePtr & resourcePtr);
};

} // namespace QL


#endif // QL_OBJECT_FILE_H;
