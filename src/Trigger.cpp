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

#include <OGRE/Ogre.h> // datastreamptr
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <lua.hpp>
#include <luabind/luabind.hpp>
#include "Trigger.h"
#include "Script/Script.h"
#include "Physics.h"

#include <cstdio>
using namespace QL;
using namespace Ogre;

Trigger::Trigger(Physics * physics, btCollisionShape * colShape, const btVector3 & origin, Script * script)
{
	m_physics = physics;
	m_ghostObject = new btPairCachingGhostObject;
	m_script = script;
	m_triggerEnter = false;

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(origin);

	m_ghostObject->setWorldTransform(transform);
	m_ghostObject->setCollisionShape(colShape);

	// Only generate contacts, but don't resolve them.
	m_ghostObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

	// Generate contacts only with character controller.
	m_physics->getDynamicsWorld()->addCollisionObject(m_ghostObject, btBroadphaseProxy::SensorTrigger,
		btBroadphaseProxy::CharacterFilter);

	m_physics->getDynamicsWorld()->addAction(static_cast<btActionInterface*>(this));
}

Trigger::~Trigger()
{
	m_physics->getDynamicsWorld()->removeCollisionObject(m_ghostObject);
	delete m_ghostObject;
}

void Trigger::updateAction(btCollisionWorld * collisionWorld, btScalar dt)
{
	btManifoldArray manifoldArray;
	btBroadphasePairArray & pairArray = m_ghostObject->getOverlappingPairCache()->getOverlappingPairArray();
	int numPairs = pairArray.size();

	if (numPairs == 0 && m_triggerEnter)
	{
		luabind::call_function<void>(m_script->getLuaState(), "triggerExit");
		m_triggerEnter = false;
	}
	else if (numPairs && m_triggerEnter)
		return;

	for (int i = 0; i < numPairs; i++)
	{
		manifoldArray.resize(0);

		const btBroadphasePair & pair = pairArray[i];
		btBroadphasePair * collisionPair =
			m_physics->getDynamicsWorld()->getPairCache()->findPair(pair.m_pProxy0, pair.m_pProxy1);

		if (!collisionPair)
			continue;

		if (collisionPair->m_algorithm)
			collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

		int manifoldSize = manifoldArray.size();
		for (int j = 0; j < manifoldSize; j++)
		{
			btPersistentManifold * manifold = manifoldArray[j];

			int numContacts = manifold->getNumContacts();
			for (int k = 0; k < numContacts; k++)
			{
				const btManifoldPoint & point = manifold->getContactPoint(k);

				if (point.getDistance() < 0)
				{
					luabind::call_function<void>(m_script->getLuaState(), "triggerEnter");
					m_triggerEnter = true;
				}
			}
		}
	}
}

void Trigger::debugDraw(btIDebugDraw * debugDrawer)
{}
