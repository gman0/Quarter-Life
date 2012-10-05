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



#ifndef QL_TRIGGER_H
#define QL_TRIGGER_H

#include <bullet/LinearMath/btVector3.h>
#include <bullet/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h>
#include <bullet/BulletDynamics/Dynamics/btActionInterface.h>
#include <OGRE/OgreSceneNode.h>

class btPairCachingGhostObject;
class btCollisionShape;
class btCollisionWorld;
class btIDebugDraw;

namespace QL
{

class Physics;
class Script;

/*
 * A trigger is a kind of a scene object which acts as a proximity sensor.
 * Basically, it's a ghost object with a sphere collision shape which
 * generates contacts, however doesn't respond to them. Note that triggers
 * generate contacts only with player (character controller).
 *
 * Triggers use Script class which handles script callbacks. Each script
 * callback handles a single trigger event.
 * We have 2 types of a trigger events:
 * 		o triggerEnter
 * 		o triggerExit
 * When a certain event occurs, the corresponing script callback is used.
 *
 * So if the player steps into a trigger, a contact is generated meaning
 * that event triggerEnter has occured. Afterwards, we call function
 * "triggerEnter()" from the script (using the Script class).
 * Likewise, when the player steps out of the trigger, the function
 * "triggerExit()" is called from the script.
 */
class Trigger : public btActionInterface
{
private:
	Physics * m_physics;
	btPairCachingGhostObject * m_ghostObject;

	/*
	 * We use only one instance of btManifoldArray troughout the whole
	 * lifetime of a trigger to avoid unneccesary allocations/deallocations.
	 */
	btManifoldArray m_manifoldArray;

	Script * m_script;

	bool m_triggerEnter;

public:
	Trigger(Physics * physics, btCollisionShape * colShape, const btVector3 & origin, Script * script);
	~Trigger();

	virtual void updateAction(btCollisionWorld * collisionWorld, btScalar dt);
	virtual void debugDraw(btIDebugDraw * debugDrawer);

private:
	void callback();
};

} // namespace QL

#endif // QL_TRIGGER_H
