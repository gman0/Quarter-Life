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


#include <OGRE/OgreArchiveManager.h>
#include <iostream>
#include "ObjectFileManager.h"

using namespace QL;
using namespace Ogre;
using namespace std;

template<> ObjectFileManager * Singleton<ObjectFileManager>::msSingleton = 0;


ObjectFileManager & ObjectFileManager::getSingleton()
{
	assert(msSingleton);
	return *msSingleton;
}

ObjectFileManager * ObjectFileManager::getSingletonPtr()
{
	return msSingleton;
}

ObjectFileManager::ObjectFileManager()
{
	mResourceType = "ObjFileSystem";
	mLoadOrder = 30;
	ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);

	m_archiveFactory = new ObjectFileArchiveFactory;
	ArchiveManager::getSingleton().addArchiveFactory(m_archiveFactory);
}

ObjectFileManager::~ObjectFileManager()
{
	// delete m_archiveFactory; FIXME: SIGSEGV
	ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

ObjectFilePtr ObjectFileManager::load(const String & name, const Ogre::String & group)
{
	ObjectFilePtr objFilePtr = getByName(name);

	if (objFilePtr.isNull())
		objFilePtr = create(name, group);

	objFilePtr->load();

	return objFilePtr;
}

Resource * ObjectFileManager::createImpl(const String & name, ResourceHandle handle,
		const String & group, bool isManual, ManualResourceLoader * loader,
		const NameValuePairList * createParams)
{
	return new ObjectFile(this, name, handle, group, isManual, loader);
}
