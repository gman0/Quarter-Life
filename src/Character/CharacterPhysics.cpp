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
    
    Part of this source code is from Bullet's btKinematicCharacterPhysics
*/

#include <bullet/btBulletDynamicsCommon.h>
#include "Character/CharacterPhysics.h"
#include "Physics.h"

#include <cstdio>

using namespace QL;

class KinematicClosestNotMeRayResultCallback : public btCollisionWorld::ClosestRayResultCallback
{
protected:
	btCollisionObject * m_me;

public:
	KinematicClosestNotMeRayResultCallback(btCollisionObject * me) :
		btCollisionWorld::ClosestRayResultCallback(btVector3(0, 0, 0), btVector3(0, 0, 0))
	{
		m_me = me;
	}

	virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult & rayResult, bool normalInWorldSpace)
	{
		if (rayResult.m_collisionObject == m_me)
			return 1.0;

		return ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
	}
};

class KinematicClosestNotMeConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback
{
protected:
	btCollisionObject * m_me;
	const btVector3 m_up;
	btScalar m_minSlopeDot;

public:
	KinematicClosestNotMeConvexResultCallback(btCollisionObject * me, const btVector3 & up,
												btScalar minSlopeDot) :
		btCollisionWorld::ClosestConvexResultCallback(btVector3(0, 0, 0), btVector3(0, 0, 0)),
		m_me(me), m_up(up), m_minSlopeDot(minSlopeDot)
	{}

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult & convexResult,
										bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == m_me ||
			!convexResult.m_hitCollisionObject->hasContactResponse())
			return 1.0;

		btVector3 hitNormalWorld;

		if (normalInWorldSpace)
			hitNormalWorld = convexResult.m_hitNormalLocal;
		else
		{
			// need to transform normal into world space
			hitNormalWorld = convexResult.m_hitCollisionObject->getWorldTransform().getBasis()
								* convexResult.m_hitNormalLocal;
		}

		btScalar dotUp = m_up.dot(hitNormalWorld);

		if (dotUp < m_minSlopeDot)
			return 1.0;

		return ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
	}
};

static btVector3 getNormalizedVector(const btVector3 & vec)
{
	btVector3 n = vec.normalized();

	if (n.fuzzyZero())
		n.setValue(0, 0, 0);

	return n;
}

const btVector3 s_upAxisDirection[3] = {
	btVector3(1, 0, 0),
	btVector3(0, 1, 0),
	btVector3(0, 0, 1)
};




CharacterPhysics::CharacterPhysics(Physics * physics)
{
	m_upAxis = 1;
	m_addedMargin = 0.02;
	m_walkDirection.setValue(0, 0, 0);
	m_walkSpeed = 0;
	// m_stepHeight = 0.35;
	m_stepHeight = 2.2;
	m_currentStepOffset = 0;
	m_turnAngle = 0.0;
	m_velocityTimeInterval = 0.0;
	m_verticalVelocity = 0.0;
	m_verticalOffset = 0.0;
	m_gravity = 9.8 * 3;
	m_fallSpeed = 55.0;
	m_jumpSpeed = 10.0;
	m_mass = 30;
	m_wasOnGround = false;
	m_wasJumping = false;
	m_wantStand = false;
	setMaxSlope(btRadians(45.0));

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0, 220, 0));

	m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setWorldTransform(startTransform);
	btScalar characterWidth = 1.75;
	btScalar characterHeight = 2.75;
	m_shapes[0] = new btCapsuleShape(characterWidth, characterHeight);
	m_shapes[1] = new btCapsuleShape(characterWidth, characterWidth / 2.5);
	m_colShape = m_shapes[0];
	m_standingHeight = (m_colShape->getHalfHeight() + m_colShape->getRadius()) * 2;
	m_crouchingHeight = (m_shapes[1]->getHalfHeight() + m_shapes[1]->getRadius()) * 2;


	m_ghostObject->setCollisionShape(m_shapes[0]);
	m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	// btScalar h = m_colShape->getRadius() * 2 + m_colShape->getHalfHeight() ;
	// printf("set: %f calculated %f\n", m_standingHeight, h);

	m_physics = physics;

	m_physics->getDynamicsWorld()->addCollisionObject(m_ghostObject,
		btBroadphaseProxy::CharacterFilter,
		btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter
		| btBroadphaseProxy::CharacterFilter | btBroadphaseProxy::SensorTrigger);
	m_physics->getDynamicsWorld()->addAction(static_cast<btActionInterface*>(this));
}

CharacterPhysics::~CharacterPhysics()
{
	delete m_shapes[1];
	delete m_shapes[0];

	m_physics->getDynamicsWorld()->removeCollisionObject(m_ghostObject);

	delete m_ghostObject;
}

btVector3 CharacterPhysics::computeReflectionDirection(const btVector3 & direction, const btVector3& normal)
{
	return direction - (btScalar(2.0) * direction.dot(normal)) * normal;
}

btVector3 CharacterPhysics::parallelComponent(const btVector3 & direction, const btVector3 & normal)
{
	btScalar magnitude = direction.dot(normal);
	return normal * magnitude;
}

btVector3 CharacterPhysics::perpendicularComponent(const btVector3 & direction, const btVector3 & normal)
{
	return direction - parallelComponent(direction, normal);
}

btPairCachingGhostObject * CharacterPhysics::getGhostObject()
{
	return m_ghostObject;
}

bool CharacterPhysics::recoverFromPenetration(btCollisionWorld * collisionWorld)
{
	bool penetration = false;

	collisionWorld->getDispatcher()->dispatchAllCollisionPairs(
		m_ghostObject->getOverlappingPairCache(),
		collisionWorld->getDispatchInfo(),
		collisionWorld->getDispatcher()
	);

	m_currentPosition = m_ghostObject->getWorldTransform().getOrigin();

	btScalar maxPenetration = 0.0;

	int overlappingPairs = m_ghostObject->getOverlappingPairCache()->getNumOverlappingPairs();
	for (int i = 0; i < overlappingPairs; i++)
	{
		m_manifoldArray.resize(0);

		btBroadphasePair * collisionPair =
			&m_ghostObject->getOverlappingPairCache()->getOverlappingPairArray()[i];

		// Does the other object respone?
		if (!((btCollisionObject*)collisionPair->m_pProxy0->m_clientObject)->hasContactResponse() ||
			!((btCollisionObject*)collisionPair->m_pProxy1->m_clientObject)->hasContactResponse())
			continue;

		if (collisionPair->m_algorithm)
			collisionPair->m_algorithm->getAllContactManifolds(m_manifoldArray);

		int manifoldArraySize = m_manifoldArray.size();
		for (int j = 0; j < manifoldArraySize; j++)
		{
			btPersistentManifold * manifold = m_manifoldArray[j];
			btScalar directionSign = (manifold->getBody0() == m_ghostObject) ? -1.0 : 1.0;

			int numContacts = manifold->getNumContacts();
			for (int k = 0; k < numContacts; k++)
			{
				const btManifoldPoint & pt = manifold->getContactPoint(k);

				btScalar dist = pt.getDistance();

				// if (dist < 0.0)
				if (fabs(dist) > m_addedMargin)
				{
					if (dist < maxPenetration)
					{
						maxPenetration = dist;
						m_touchingNormal = pt.m_normalWorldOnB * directionSign;
					}

					m_currentPosition += pt.m_normalWorldOnB * directionSign * dist * 0.2;
					penetration = true;
				}
			}
		}
	}

	btTransform newTransform = m_ghostObject->getWorldTransform();
	newTransform.setOrigin(m_currentPosition);
	m_ghostObject->setWorldTransform(newTransform);

	return penetration;
}

void CharacterPhysics::stepUp(btCollisionWorld * world)
{
	btTransform start, end;
	m_targetPosition = m_currentPosition + s_upAxisDirection[m_upAxis] * (m_stepHeight +
						((m_verticalOffset > 0.0) ? m_verticalOffset : 0.0));

	start.setIdentity();
	end.setIdentity();

	start.setOrigin(m_currentPosition + s_upAxisDirection[m_upAxis] * (m_colShape->getMargin()
					+ m_addedMargin));
	end.setOrigin(m_targetPosition);

	KinematicClosestNotMeConvexResultCallback callback(m_ghostObject, -s_upAxisDirection[m_upAxis],
														btScalar(0.7071));

	callback.m_collisionFilterGroup = m_ghostObject->getBroadphaseHandle()->m_collisionFilterGroup;
	callback.m_collisionFilterMask = m_ghostObject->getBroadphaseHandle()->m_collisionFilterMask;

	m_ghostObject->convexSweepTest(m_colShape, start, end, callback,
		world->getDispatchInfo().m_allowedCcdPenetration);

	if (callback.hasHit())
	{
		// Only modify the position if the hit was a slope and not a wall or ceiling.
		if (callback.m_hitNormalWorld.dot(s_upAxisDirection[m_upAxis]) > 0.0)
		{
			m_currentStepOffset = m_stepHeight * callback.m_closestHitFraction;
			m_currentPosition.setInterpolate3(m_currentPosition, m_targetPosition,
												callback.m_closestHitFraction);
		}

		m_verticalVelocity = 0.0;
		m_verticalOffset = 0.0;
	}
	else
	{
		m_currentStepOffset = m_stepHeight;
		m_currentPosition = m_targetPosition;
	}
}

void CharacterPhysics::updateTargetPositionBasedOnCollision(const btVector3 & hitNormal,
																btScalar tangentMag,
																btScalar normalMag)
{
	btVector3 movementDirection = m_targetPosition - m_currentPosition;
	btScalar movementLength = movementDirection.length();

	if (movementLength > SIMD_EPSILON)
	{
		movementDirection.normalize();

		btVector3 reflectDir = computeReflectionDirection(movementDirection, hitNormal);
		reflectDir.normalize();

		btVector3 parallelDir, perpendicularDir;

		parallelDir = parallelComponent(reflectDir, hitNormal);
		perpendicularDir = perpendicularComponent(reflectDir, hitNormal);

		m_targetPosition = m_currentPosition;

		if (normalMag)
		{
			btVector3 perpComponent = perpendicularDir * btScalar(normalMag * movementLength);
			m_targetPosition += perpComponent;
		}
	}
}

void CharacterPhysics::stepForwardAndStrafe(btCollisionWorld * collisionWorld, const btVector3 & walkMove)
{
	btTransform start, end;
	m_targetPosition = m_currentPosition + walkMove;

	start.setIdentity();
	end.setIdentity();

	btScalar fraction = 1.0;
	btScalar distance2 = (m_currentPosition - m_targetPosition).length2();

	if (m_touchingContact)
	{
		if (m_normalizedDirection.dot(m_touchingNormal) > 0.0)
			updateTargetPositionBasedOnCollision(m_touchingNormal);
	}

	btScalar margin = m_colShape->getMargin();

	int maxIter = 10;

	while (fraction > btScalar(0.01) && maxIter-- > 0)
	{
		start.setOrigin(m_currentPosition);
		end.setOrigin(m_targetPosition);

		btVector3 sweepDirNegative(m_currentPosition - m_targetPosition);

		KinematicClosestNotMeConvexResultCallback callback(m_ghostObject, sweepDirNegative, 0.0);
		callback.m_collisionFilterGroup = m_ghostObject->getBroadphaseHandle()->m_collisionFilterGroup;
		callback.m_collisionFilterMask = m_ghostObject->getBroadphaseHandle()->m_collisionFilterMask;

		m_colShape->setMargin(margin + m_addedMargin);

		m_ghostObject->convexSweepTest(m_colShape, start, end, callback,
			collisionWorld->getDispatchInfo().m_allowedCcdPenetration);

		m_colShape->setMargin(margin);

		fraction -= callback.m_closestHitFraction;

		if (callback.hasHit())
		{
			updateTargetPositionBasedOnCollision(callback.m_hitNormalWorld);
			btVector3 currentDir = m_targetPosition - m_currentPosition;
			distance2 = currentDir.length2();

			if (distance2 > SIMD_EPSILON)
			{
				currentDir.normalize();

				/* See Quake2: "If velocity is against original velocity, stop ead to avoid tiny
				 * oscilations in sloping corners." */
				if (currentDir.dot(m_normalizedDirection) <= 0.0)
					break;
			}
			else
				break;
		}
		else
			m_currentPosition = m_targetPosition; // we moved whole way
	}
}

void CharacterPhysics::stepDown(btCollisionWorld * collisionWorld, btScalar dt)
{
	btTransform start, end;

	btScalar downVelocity = (m_verticalVelocity < 0.0 ? -m_verticalVelocity : 0.0) * dt;

	if (downVelocity > 0.0 && downVelocity < m_stepHeight && (m_wasOnGround || !m_wasJumping))
		downVelocity = m_stepHeight;

	btVector3 stepDrop = s_upAxisDirection[m_upAxis] * (m_currentStepOffset + downVelocity);
	m_targetPosition -= stepDrop;

	start.setIdentity();
	end.setIdentity();

	start.setOrigin(m_currentPosition);
	end.setOrigin(m_targetPosition);

	KinematicClosestNotMeConvexResultCallback callback(m_ghostObject, s_upAxisDirection[m_upAxis],
														m_maxSlopeCosine);

	callback.m_collisionFilterGroup = m_ghostObject->getBroadphaseHandle()->m_collisionFilterGroup;
	callback.m_collisionFilterMask = m_ghostObject->getBroadphaseHandle()->m_collisionFilterMask;

	m_ghostObject->convexSweepTest(m_colShape, start, end, callback,
		collisionWorld->getDispatchInfo().m_allowedCcdPenetration);

	if (callback.hasHit())
	{
		// we dropped a fraction of the height -> hit floor
		m_currentPosition.setInterpolate3(m_currentPosition, m_targetPosition, callback.m_closestHitFraction);
		m_verticalVelocity = 0.0;
		m_verticalOffset = 0.0;
		m_wasJumping = false;
	}
	else
		m_currentPosition = m_targetPosition; // we dropped the full height
}

void CharacterPhysics::setWalkDirection(const btVector3 & walkDirection)
{
	m_walkDirection = walkDirection;
	m_normalizedDirection = getNormalizedVector(m_walkDirection);
}

void CharacterPhysics::setVelocityForTimeInterval(const btVector3 & velocity, btScalar timeInterval)
{
	m_walkDirection = velocity;
	m_normalizedDirection = getNormalizedVector(m_walkDirection);
	m_velocityTimeInterval = timeInterval;
}

void CharacterPhysics::reset()
{}

void CharacterPhysics::warp(const btVector3 & origin)
{
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(origin);

	m_verticalVelocity = 0;
	m_verticalOffset = 0;

	m_walkDirection.setValue(0, 0, 0);
	
	m_ghostObject->setWorldTransform(transform);
}

void CharacterPhysics::preStep(btCollisionWorld * collisionWorld)
{
	int numPenetrationLoops = 0;
	m_touchingContact = false;

	while (recoverFromPenetration(collisionWorld) && numPenetrationLoops < 4)
	{
		numPenetrationLoops++;
		m_touchingContact = true;
	}

	m_currentPosition = m_ghostObject->getWorldTransform().getOrigin();
	m_targetPosition = m_currentPosition;
}

void CharacterPhysics::playerStep(btCollisionWorld * collisionWorld, btScalar dt)
{
	m_wasOnGround = onGround();

	// Update fall velocity.
	m_verticalVelocity -= m_gravity * dt;

	if (m_verticalVelocity > 0.0 && m_verticalVelocity > m_jumpSpeed)
		m_verticalVelocity = m_jumpSpeed;

	if (m_verticalVelocity < 0.0 && btFabs(m_verticalVelocity) > btFabs(m_fallSpeed))
		m_verticalVelocity = -btFabs(m_fallSpeed);

	m_verticalOffset = m_verticalVelocity * dt;

	btTransform transform;
	transform = m_ghostObject->getWorldTransform();

	stepUp(collisionWorld);

	btScalar walkSpeed = dt * 10;
	btVector3 walkVelocity = m_walkDirection * walkSpeed;

	stepForwardAndStrafe(collisionWorld, walkVelocity);

	stepDown(collisionWorld, dt);

	if (m_wantStand)
		stand(collisionWorld);

	transform.setOrigin(m_currentPosition);
	m_ghostObject->setWorldTransform(transform);
}

void CharacterPhysics::setFallSpeed(btScalar fallSpeed)
{
	m_fallSpeed = fallSpeed;
}

void CharacterPhysics::setJumpSpeed(btScalar jumpSpeed)
{
	m_jumpSpeed = jumpSpeed;
}

void CharacterPhysics::setMaxJumpHeight(btScalar maxJumpHeight)
{
	m_maxJumpHeight = maxJumpHeight;
}

bool CharacterPhysics::canJump() const
{
	return onGround();
}

void CharacterPhysics::jump()
{
	if (!canJump())
		return;

	m_verticalVelocity = m_jumpSpeed;
	m_wasJumping = true;
}

void CharacterPhysics::crouch()
{
	m_ghostObject->setCollisionShape(m_shapes[1]); // Change the collision shape to crouching.
	m_colShape = m_shapes[1];

	// Move the character lower so we don't start falling.
	m_currentPosition.setY(m_currentPosition.y() - m_standingHeight + m_crouchingHeight);
}

void CharacterPhysics::crouchEnd()
{
	m_wantStand = true;
}

bool CharacterPhysics::canStand(btCollisionWorld * collisionWorld) const
{
	btCapsuleShape * standingColShape = m_shapes[0];

	if (m_ghostObject->getCollisionShape() != standingColShape) // If we aren't already standing.
	{
		// Perform a ray test to check if we have enough space above us to stand up.

		btTransform start, end;

		start.setIdentity();
		end.setIdentity();

		btScalar standHeight = (standingColShape->getHalfHeight() + standingColShape->getRadius()) * 2;
		btScalar crouchHeight = (m_shapes[1]->getHalfHeight() + m_shapes[1]->getRadius()) * 2;

		btVector3 endOrigin(m_currentPosition);
		endOrigin.setY(endOrigin.y() - crouchHeight + standHeight);

		start.setOrigin(m_currentPosition);
		end.setOrigin(endOrigin);

		KinematicClosestNotMeConvexResultCallback callback(m_ghostObject, s_upAxisDirection[m_upAxis],
															m_maxSlopeCosine);

		callback.m_collisionFilterGroup = m_ghostObject->getBroadphaseHandle()->m_collisionFilterGroup;
		callback.m_collisionFilterMask = m_ghostObject->getBroadphaseHandle()->m_collisionFilterMask;

		m_ghostObject->convexSweepTest(m_colShape, start, end, callback,
			collisionWorld->getDispatchInfo().m_allowedCcdPenetration);

		return !callback.hasHit();
	}

	return true;
}

void CharacterPhysics::stand(btCollisionWorld * collisionWorld)
{
	if (!canStand(collisionWorld))
		return;

	m_ghostObject->setCollisionShape(m_shapes[0]);
	m_colShape = m_shapes[0];

	// Move character up to avoid interpenetration.
	// m_currentPosition.setY(m_currentPosition.y() - m_crouchingHeight + m_standingHeight);

	m_wantStand = false;
}

void CharacterPhysics::setGravity(btScalar gravity)
{
	m_gravity = gravity;
}

btScalar CharacterPhysics::getGravity() const
{
	return m_gravity;
}

void CharacterPhysics::setMaxSlope(btScalar slopeRadians)
{
	m_maxSlopeRadians = slopeRadians;
	m_maxSlopeCosine = btCos(slopeRadians);
}

btScalar CharacterPhysics::getMaxSlope() const
{
	return m_maxSlopeRadians;
}

bool CharacterPhysics::onGround() const
{
	return (m_verticalVelocity == 0.0 && m_verticalOffset == 0.0);
}

void CharacterPhysics::debugDraw(btIDebugDraw * debugDraw)
{}

void CharacterPhysics::updateAction(btCollisionWorld * collisionWorld, btScalar dt)
{
	preStep(collisionWorld);
	playerStep(collisionWorld, dt);
}

const btVector3 & CharacterPhysics::getPosition() const
{
	return m_ghostObject->getWorldTransform().getOrigin();
}
