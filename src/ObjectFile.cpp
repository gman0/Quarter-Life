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


#include <OGRE/OgreLogManager.h>
#include <OGRE/OgreArchiveManager.h>
#include <OGRE/OgreArchive.h>
#include <OGRE/OgreZip.h>
#include <OGRE/OgreFileSystem.h>
#include <iostream>
#include "ObjectFile.h"


using namespace QL;
using namespace Ogre;

using namespace std;

ObjectFile::ObjectFile(ResourceManager * creator, const String & name, ResourceHandle handle,
						const String & group, bool isManual, ManualResourceLoader * loader) :
	Resource(creator, name, handle, group, isManual, loader), m_name(name)
{
	createParamDictionary("ObjectFile");
}

ObjectFile::~ObjectFile()
{
	unload(); // <-- FIXME: SIGSEGV
}

void ObjectFile::loadImpl()
{
	LogManager::getSingleton().logMessage("Object: Loading '" + getName() + "'");

	FileInfoListPtr fiListPtr = ResourceGroupManager::getSingleton().findResourceFileInfo(getGroup(), getName());

	size_t size = fiListPtr->size();

	if (size == 0)
		throw IOException(IOException::ERR_FILE_NOT_FOUND,
							"Cannot find object '" + getName() + "'", "QL::ObjectFile::loadImpl",
							__FILE__, __LINE__);

	if (size > 1)
		LogManager::getSingleton().logMessage("WARNING: Object '" + getName()
												+ "' is ambigouous! The first one found will be used.");

	FileInfo fi = (*fiListPtr.get())[0];
	String path = fi.archive->getName() + "/" + fi.filename;
	m_size = fi.uncompressedSize;

	m_archive = ArchiveManager::getSingleton().load(path, "Zip");
}

void ObjectFile::unloadImpl()
{
	static_cast<ZipArchive*>(m_archive)->unload();
}

size_t ObjectFile::calculateSize() const
{
	return m_size;
}

Archive * ObjectFile::getArchive()
{
	return m_archive;
}


ObjectFilePtr::ObjectFilePtr() : Ogre::SharedPtr<ObjectFile>()
{}

ObjectFilePtr::ObjectFilePtr(ObjectFile * ptr) : Ogre::SharedPtr<ObjectFile>(ptr)
{}

ObjectFilePtr::ObjectFilePtr(const Ogre::ResourcePtr & resourceRef) : Ogre::SharedPtr<ObjectFile>()
{
	if (resourceRef.isNull())
		return;

	OGRE_LOCK_MUTEX(*resourceRef.OGRE_AUTO_MUTEX_NAME)
	OGRE_COPY_AUTO_SHARED_MUTEX(resourceRef.OGRE_AUTO_MUTEX_NAME)

	pRep = static_cast<ObjectFile*>(resourceRef.getPointer());
	pUseCount = resourceRef.useCountPointer();
	useFreeMethod = resourceRef.freeMethod();

	if (pUseCount)
		++(*pUseCount);
}

ObjectFilePtr & ObjectFilePtr::operator=(const Ogre::ResourcePtr & resourcePtr)
{
	if (pRep == static_cast<ObjectFile*>(resourcePtr.getPointer()))
		return *this;

	release();

	if (resourcePtr.isNull())
		return *this;
	
	OGRE_LOCK_MUTEX(*resourcePtr.OGRE_AUTO_MUTEX_NAME)
	OGRE_COPY_AUTO_SHARED_MUTEX(resourcePtr.OGRE_AUTO_MUTEX_NAME)

	pRep = static_cast<ObjectFile*>(resourcePtr.getPointer());
	pUseCount = resourcePtr.useCountPointer();
	useFreeMethod = resourcePtr.freeMethod();

	if (pUseCount)
		++(*pUseCount);

	return *this;
}
