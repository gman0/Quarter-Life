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


#include <OGRE/OgreMeshSerializer.h>
#include <OGRE/OgreMeshManager.h>
#include <bullet/Serialize/BulletFileLoader/btBulletFile.h>
#include <bullet/Serialize/BulletWorldImporter/btBulletWorldImporter.h>
#include <iostream>
#include <cstdio>
#include "ObjectManager.h"
#include "helpers/Convert.h"

using namespace QL;
using namespace Ogre;
using namespace std;

size_t ObjectManager::ms_totalColShapeBufferSize = 1024 * 1024 * 5;

ObjectManager::ObjectManager(Physics * physics, TriggerManager * triggerManager,
							ScriptManager * scriptManager) :
	m_sceneManager(0), m_physics(physics), m_triggerManager(triggerManager),
	m_scriptManager(scriptManager)
{
	m_meshSerializer = new MeshSerializer;
	m_bulletWorldImporter = new btBulletWorldImporter(physics->getDynamicsWorld());
	m_colShapeBuffer = new unsigned char[ms_totalColShapeBufferSize];
}

ObjectManager::~ObjectManager()
{
	removeAllObjects();
	delete [] m_colShapeBuffer;
	delete m_meshSerializer;
	delete m_bulletWorldImporter;
}

void ObjectManager::setSceneManager(SceneManager * sceneManager)
{
	m_sceneManager = sceneManager;
}

void ObjectManager::removeAllObjects()
{
	for (int i = m_objectList.size() - 1; i >= 0; i--)
		delete m_objectList[i];

	// TODO: vymazat meshe aj z MeshManager, SceneNode, rigid bodies

	m_objectList.resize(0);
}

Object * ObjectManager::addObject(ObjectFilePtr & objFilePtr, SceneNode * node,
												 btScalar mass)
{
	assert(m_sceneManager);

	Archive * archive = objFilePtr->getArchive();
	FileInfoListPtr fiListPtr = archive->listFileInfo();
	ObjectInfo objInfo = getObjectInfo(fiListPtr, archive->getName());
	Object * obj = new Object;
	memset(obj, 0, sizeof(Object));

	obj->sceneNode = node;

	if (objInfo.type & OT_RENDERABLE)
		loadMeshFile(archive, objInfo.meshFileName, obj);

	if (objInfo.type & OT_RIGIDBODY)
	{
		btCollisionShape * colShape = loadBulletFile(archive, objInfo.collisionShapeFileName, obj);

		if (colShape)
		{
			btVector3 origin(convert(node->getPosition()));
			btQuaternion rotation(convert(node->getOrientation()));
			btVector3 scale(convert(node->getScale()));

			btTransform transform;
			transform.setIdentity();
			transform.setOrigin(origin);
			transform.setRotation(rotation);

			colShape->setLocalScaling(scale);

			m_physics->addRigidBody(transform, colShape, mass, node, false);
		}
	}

	if (objInfo.type & OT_TRIGGER)
		loadScriptFile(archive, objInfo.scriptFileName, obj);

	return obj;
}

void ObjectManager::loadMeshFile(const Archive * archive, const String & name, Object * obj)
{
	Mesh mesh(MeshManager::getSingletonPtr(), name, 0, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	DataStreamPtr streamPtr = archive->open(name);
	m_meshSerializer->importMesh(streamPtr, &mesh);
	streamPtr->close();

	obj->entity = m_sceneManager->createEntity(mesh.clone(name)); // FIXME: maybe use manual loader?
	obj->sceneNode->attachObject(obj->entity);

}

btCollisionShape * ObjectManager::loadBulletFile(const Archive * archive, const String & name, Object * obj)
{
	DataStreamPtr streamPtr = archive->open(name);
	size_t colShapeBufferSize = streamPtr->size();
	btCollisionShape * colShape = 0;

	streamPtr->read(m_colShapeBuffer, colShapeBufferSize);

	bParse::btBulletFile file((char*)m_colShapeBuffer, colShapeBufferSize);

	streamPtr->close();

	if ((file.getFlags() & bParse::FD_OK))
	{
		file.parse(false);
		m_bulletWorldImporter->convertAllObjects(&file);

		int index = m_bulletWorldImporter->getNumCollisionShapes() - 1;
		colShape = m_bulletWorldImporter->getCollisionShapeByIndex(index);
	}

	if (!colShape)
		LogManager::getSingleton().logMessage("Cannot read '" + name
										+ "' from '" + archive->getName()
										+ "' archive, rigidbody will not be created!");

	return colShape;
}

void ObjectManager::loadScriptFile(const Archive * archive, const String & name, Object * obj)
{
	DataStreamPtr streamPtr = archive->open(name);
	Script * script = m_scriptManager->createScript(streamPtr);
	streamPtr->close();

	obj->trigger = m_triggerManager->createTrigger(convert(obj->sceneNode->getPosition()), script);
}

ObjectManager::ObjectInfo ObjectManager::getObjectInfo(const FileInfoListPtr & fiListPtr, const String & name)
{
	ObjectInfo objInfo;
	unsigned char flags = 0;

	static const char * scriptExtension = "lua";

	for (FileInfoList::const_iterator it = fiListPtr->begin(); it != fiListPtr->end(); ++it)
	{
		String basename, extension, filename = it->basename;
		StringUtil::splitBaseFilename(filename, basename, extension);

		if (extension == "mesh")
		{
			objInfo.meshFileName = filename;
			flags |= ObjectInfo::OI_MESH;
		}
		else if (extension == "bullet")
		{
			objInfo.collisionShapeFileName = filename;
			flags |= ObjectInfo::OI_COLSHAPE;
		}
		else if (extension == scriptExtension)
		{
			objInfo.scriptFileName = filename;
			flags |= ObjectInfo::OI_SCRIPT;
		}
		else
			LogManager::getSingleton().logMessage("Unknown type of file '" + filename + "' in '" + name
												+ "' archive, ignoring.");
	}

	objInfo.type = getObjectType(flags, name);

	return objInfo;
}

ObjectManager::OBJECT_TYPE ObjectManager::getObjectType(unsigned char flags, const String & objFileName)
{
	if (!flags)
		return OT_UNKNOWN;

	if (flags & ObjectInfo::OI_SCRIPT)
	{
		if (flags & ObjectInfo::OI_MESH)
			return OT_RENDERABLE_TRIGGER;
		else
			return OT_TRIGGER;
	}
	else
	{
		if (flags & ObjectInfo::OI_MESH)
		{
			if (flags & ObjectInfo::OI_COLSHAPE)
				return OT_RENDERABLE_RIGIDBODY;
			else
				return OT_RENDERABLE;
		}
		else
		{
			if (flags & ObjectInfo::OI_COLSHAPE)
				return OT_RIGIDBODY;
			else
				return OT_UNKNOWN;
		}
	}
}
