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



#ifndef QL_OBJECT_FILE_MANAGER_H
#define QL_OBJECT_FILE_MANAGER_H

#include <OGRE/OgreResourceManager.h>
#include "ObjectFile.h"
#include "ObjectFileArchiveFactory.h"

namespace QL
{

class ObjectFileManager : public Ogre::ResourceManager, public Ogre::Singleton<ObjectFileManager>
{
private:
	ObjectFileArchiveFactory * m_archiveFactory;

public:
	ObjectFileManager();
	virtual ~ObjectFileManager();

	virtual ObjectFilePtr load(const Ogre::String & name, const Ogre::String & group);

	static ObjectFileManager & getSingleton();
	static ObjectFileManager * getSingletonPtr();

protected:
	virtual Ogre::Resource * createImpl(const Ogre::String & name, Ogre::ResourceHandle handle,
				const Ogre::String & group, bool isManual, Ogre::ManualResourceLoader * loader,
				const Ogre::NameValuePairList * createParams);
};


} // namespace QL

#endif // QL_OBJECT_FILE_MANAGER_H
