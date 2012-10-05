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


#ifndef QL_PHYSICS_H
#define QL_PHYSICS_H

#include <bullet/btBulletDynamicsCommon.h>
#include <OGRE/OgreSceneNode.h>

class btGhostPairCallback;

namespace QL
{

class Physics
{
private:
	// Storage for all collision shapes so we can delete them in the destructor.
	btAlignedObjectArray<btCollisionShape*> m_collisionShapes;

	btBroadphaseInterface * m_broadphase;
	btCollisionDispatcher * m_dispatcher;
	btConstraintSolver * m_constraintSolver;
	btDefaultCollisionConfiguration * m_colConfig;
	btDiscreteDynamicsWorld * m_dynamicsWorld;
	btGhostPairCallback * m_ghostPairCallback;

public:
	Physics();
	~Physics();

	/*
	 * This method automatically by defaultadds (addColShape) the collision shape into m_collisionShapes,
	 * therefore it will be deleted in the destructor.
	 */
	btRigidBody * addRigidBody(const btTransform & transform, btCollisionShape * shape,
								btScalar mass, Ogre::SceneNode * node = 0, bool addColShape = true);

	btDiscreteDynamicsWorld * getDynamicsWorld();
	btBroadphaseInterface * getBroadphase();
	void addCollisionShape(btCollisionShape * shape);
};


class MotionState : public btMotionState
{
private:
	btTransform m_transform;
	Ogre::SceneNode * m_sceneNode;

public:
	MotionState(const btTransform & initialPos, Ogre::SceneNode * node = 0);
	virtual ~MotionState() {}

	virtual void getWorldTransform(btTransform & worldTransform) const;
	virtual void setWorldTransform(const btTransform & worldTransform);

	void setNode(Ogre::SceneNode * node);
	const btTransform & getWorldTransform() const;
};


} // namespace QL

#endif // QL_PHYSICS_H
