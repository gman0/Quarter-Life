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


#include <OGRE/OgreResourceGroupManager.h>
#include <OGRE/OgreConfigFile.h>
#include "ResourceManager.h"

using namespace QL;

ResourceManager::ResourceManager(const char * resConfigFileName) :
	m_resourceConfigFileName(resConfigFileName), m_objFileManager(0)
{}

ResourceManager::~ResourceManager()
{
	delete m_objFileManager;
}

void ResourceManager::initialiseResources()
{
	registerResourceTypes();
	readResConfigFile();

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void ResourceManager::registerResourceTypes()
{
	m_objFileManager = new ObjectFileManager;
}

void ResourceManager::readResConfigFile()
{
	Ogre::ConfigFile cf;
	cf.load(m_resourceConfigFileName);

	Ogre::ConfigFile::SectionIterator sectionIter = cf.getSectionIterator();

	Ogre::String sectionName, typeName, dataName;
	Ogre::ConfigFile::SettingsMultiMap * settings;
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	while (sectionIter.hasMoreElements())
	{
		sectionName = sectionIter.peekNextKey();
		settings = sectionIter.getNext();

		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			dataName = i->second;

			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(dataName, typeName, sectionName);
		}
	}
}

void ResourceManager::initialiseResourceGroup(const char * groupName, const char * path, const char * type)
{
	Ogre::ResourceGroupManager * resManager = Ogre::ResourceGroupManager::getSingletonPtr();

	resManager->addResourceLocation(path, type, groupName);
	resManager->initialiseResourceGroup(groupName);
}
