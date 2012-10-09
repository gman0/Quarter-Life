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

#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include "Physics.h"
#include "helpers/Convert.h"

#include <iostream>
using namespace std;

using namespace QL;

Physics::Physics()
{
	m_colConfig = new btDefaultCollisionConfiguration;
	m_dispatcher = new btCollisionDispatcher(m_colConfig);
	m_broadphase = new btDbvtBroadphase;
	m_constraintSolver = new btSequentialImpulseConstraintSolver;
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_constraintSolver, m_colConfig);

	m_dynamicsWorld->setGravity(btVector3(0, -10, 0));
	m_dynamicsWorld->getDispatchInfo().m_allowedCcdPenetration = btScalar(0.0001);

	m_ghostPairCallback = new btGhostPairCallback;
	m_broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(m_ghostPairCallback);
}

Physics::~Physics()
{
	cleanUpWorld();

	delete m_ghostPairCallback;
	delete m_dynamicsWorld;
	delete m_constraintSolver;
	delete m_broadphase;
	delete m_dispatcher;
	delete m_colConfig;
}

btRigidBody * Physics::addRigidBody(const btTransform & transform, btCollisionShape * shape,
									btScalar mass, Ogre::SceneNode * node, bool addColShape)
{
	btVector3 localInertia(0, 0, 0);

	/*
	 * A rigid body with zero mass is static, therefore we don't
	 * need to caluclate it's inertia tensor.
	 */
	if (mass)
		shape->calculateLocalInertia(mass, localInertia);

	MotionState * mt = new MotionState(transform, node);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, mt, shape, localInertia);
	btRigidBody * body = new btRigidBody(rbInfo);

	m_dynamicsWorld->addRigidBody(body);

	if (addColShape)
		m_collisionShapes.push_back(shape);
	
	return body;
}

btDiscreteDynamicsWorld * Physics::getDynamicsWorld()
{
	return m_dynamicsWorld;
}

btBroadphaseInterface * Physics::getBroadphase()
{
	return m_broadphase;
}

void Physics::addCollisionShape(btCollisionShape * shape)
{
	m_collisionShapes.push_back(shape);
}

void Physics::cleanUpWorld()
{
	for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject * obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody * rb = btRigidBody::upcast(obj);

		if (rb && rb->getMotionState())
			delete rb->getMotionState();

		m_dynamicsWorld->removeCollisionObject(obj);

		delete obj;
	}

	for (int i = m_collisionShapes.size() - 1; i >= 0; i--)
		delete m_collisionShapes[i];
	
	m_collisionShapes.resize(0);
}

/// VVV QL::MotionState VVV

MotionState::MotionState(const btTransform & initialPos, Ogre::SceneNode * node)
{
	m_transform = initialPos;
	m_sceneNode = node;
}

void MotionState::getWorldTransform(btTransform & worldTransform) const
{
	worldTransform = m_transform;
}

void MotionState::setWorldTransform(const btTransform & worldTransform)
{
	m_transform = worldTransform;

	if (m_sceneNode)
	{
		m_sceneNode->setPosition(convert(m_transform.getOrigin()));
		m_sceneNode->setOrientation(convert(m_transform.getRotation()));
	}
}

void MotionState::setNode(Ogre::SceneNode * node)
{
	m_sceneNode = node;
}

const btTransform & MotionState::getWorldTransform() const
{
	return m_transform;
}
