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
    
    Part of this source code is from Bullet's btKinematicCharacterController
*/


#ifndef QL_CHARACTER_PHYSICS_H
#define QL_CHARACTER_PHYSICS_H

#include <bullet/BulletDynamics/Character/btCharacterControllerInterface.h>
#include <bullet/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet/LinearMath/btVector3.h>

class btCapsuleShape;

namespace QL
{

class Physics;

class CharacterPhysics : public btCharacterControllerInterface
{
private:
	Physics * m_physics;

	btPairCachingGhostObject * m_ghostObject;

	btCapsuleShape * m_shapes[2]; // We are going to use 2 shapes: 1st for standing, 2nd for crouching.
	btCapsuleShape * m_colShape; // Current shape.

	// So we don't have to calulate these each time.
	btScalar m_standHeight;
	btScalar m_crouchHeight;

	int m_upAxis;

	btScalar m_mass;
	btScalar m_verticalVelocity;
	btScalar m_verticalOffset;
	btScalar m_fallSpeed;
	btScalar m_jumpSpeed;
	btScalar m_maxJumpHeight;
	btScalar m_maxSlopeRadians; // Max angle of a slope surface which we are able to walk on.
	btScalar m_maxSlopeCosine; // Cosine equivalent of m_maxSlopeRadians.
	btScalar m_gravity;
	btScalar m_turnAngle;
	btScalar m_stepHeight;

	btVector3 m_walkDirection; // The desired walk direction.
	btVector3 m_normalizedDirection;
	btScalar m_walkSpeed;

	btVector3 m_currentPosition;
	btVector3 m_targetPosition;
	btScalar m_currentStepOffset;

	btManifoldArray m_manifoldArray;

	btScalar m_addedMargin;
	bool m_touchingContact;
	btVector3 m_touchingNormal;

	bool m_wasOnGround;
	bool m_wasJumping;
	btScalar m_velocityTimeInterval;
	bool m_wantStand;



public:
	CharacterPhysics(Physics * physics);
	virtual ~CharacterPhysics();

	/** from btCharacterControllerInterface */

	virtual void updateAction(btCollisionWorld * collisionWorld, btScalar dt);
	virtual void debugDraw(btIDebugDraw * debugDraw);

	virtual void setWalkDirection(const btVector3 & walkDirection);
	virtual void setVelocityForTimeInterval(const btVector3 & velocity, btScalar timeInterval);

	virtual void reset();
	virtual void warp(const btVector3 & origin);

	virtual void preStep(btCollisionWorld * collisionWorld);
	virtual void playerStep(btCollisionWorld * collisionWorld, btScalar dt);
	virtual bool canJump() const;
	virtual void jump();
	virtual bool onGround() const;

	void crouch();

	/*
	 * This method notifies CC that we want to stand up.
	 * The actual "stand up" is performed by stand() which is called in playerStep.
	 */
	void crouchEnd();

	bool canStand(btCollisionWorld * collisionWorld) const;

	void setGravity(btScalar gravity);
	btScalar getGravity() const;

	void setMaxSlope(btScalar slopeRadians);
	btScalar getMaxSlope() const;

	void setFallSpeed(btScalar fallSpeed);
	void setMaxJumpHeight(btScalar maxJumpHeight);
	void setJumpSpeed(btScalar jumpSpeed);

	btPairCachingGhostObject * getGhostObject();

	const btVector3 & getPosition() const;


private:
	btVector3 computeReflectionDirection(const btVector3 & direction, const btVector3 & normal);
	btVector3 parallelComponent(const btVector3 & direction, const btVector3 & normal);
	btVector3 perpendicularComponent(const btVector3 & direction, const btVector3 & normal);

	bool recoverFromPenetration(btCollisionWorld * collisionWorld);
	void stepUp(btCollisionWorld * collisionWorld);
	void updateTargetPositionBasedOnCollision(const btVector3 & hitNormal,
			btScalar tangentMag = btScalar(0.0), btScalar normalMag = btScalar(1.0));
	void stepForwardAndStrafe(btCollisionWorld * collsionWorld, const btVector3 & walkMove);
	void stepDown(btCollisionWorld * collisionWorld, btScalar dt);

	void stand(btCollisionWorld * collisionWorld);
};


} // namespace QL

#endif // QL_CHARACTER_PHYSICS_H
