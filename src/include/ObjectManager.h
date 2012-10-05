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


#ifndef QL_OBJECT_MANAGER_H
#define QL_OBJECT_MANAGER_H

#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSceneManager.h>
#include <bullet/BulletCollision/CollisionShapes/btCollisionShape.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet/LinearMath/btAlignedObjectArray.h>
#include "Physics.h"
#include "TriggerManager.h"
#include "Script/ScriptManager.h"
#include "ObjectFile.h"
#include "Object.h"


namespace Ogre
{
class MeshSerializer;
}

class btBulletWorldImporter;

namespace QL
{

class ObjectManager
{
public:
	
	// Enum for deteriming what's the object's type.
	enum OBJECT_TYPE
	{
		OT_UNKNOWN = 1 << 0,
		OT_RENDERABLE = 1 << 1,
		OT_RIGIDBODY = 1 << 2,
		OT_RENDERABLE_RIGIDBODY = OT_RENDERABLE | OT_RIGIDBODY,
		OT_TRIGGER = 1 << 3,
		OT_RENDERABLE_TRIGGER = OT_RENDERABLE | OT_TRIGGER
	};

	typedef btAlignedObjectArray<Object*> ObjectList;


private:
	Ogre::SceneManager * m_sceneManager;
	Ogre::MeshSerializer * m_meshSerializer;
	btBulletWorldImporter * m_bulletWorldImporter;
	Physics * m_physics;
	TriggerManager * m_triggerManager;
	ScriptManager * m_scriptManager;
	ObjectList m_objectList;
	unsigned char * m_colShapeBuffer;
	static size_t ms_totalColShapeBufferSize;

	struct ObjectInfo
	{
		Ogre::String meshFileName;
		Ogre::String collisionShapeFileName;
		Ogre::String scriptFileName;
		OBJECT_TYPE type;

		enum FLAGS
		{
			OI_MESH =		1 << 1,
			OI_COLSHAPE =	1 << 2,
			OI_SCRIPT =		1 << 3
		};
	};

public:
	ObjectManager(Physics * physics, TriggerManager * triggerManager,
					ScriptManager * scriptManager);
	~ObjectManager();

	void setSceneManager(Ogre::SceneManager * sceneManager);

	void removeAllObjects();
	Object * addObject(ObjectFilePtr & objFilePtr, Ogre::SceneNode * node, btScalar mass);

private:
	ObjectInfo getObjectInfo(const Ogre::FileInfoListPtr & fiListPtr, const Ogre::String & name);
	OBJECT_TYPE getObjectType(unsigned char flags, const Ogre::String & objFileName);

	void loadMeshFile(const Ogre::Archive * archive, const Ogre::String & name, Object * obj);
	btCollisionShape * loadBulletFile(const Ogre::Archive * archive, const Ogre::String & name, Object * obj);
	void loadScriptFile(const Ogre::Archive * archive, const Ogre::String & name, Object * obj);
};

} // namespace QL


#endif // QL_OBJECT_MANAGER_H
