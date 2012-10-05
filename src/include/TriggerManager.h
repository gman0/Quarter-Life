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


#ifndef QL_TRIGGER_MANAGER_H
#define QL_TRIGGER_MANAGER_H

#include <bullet/LinearMath/btAlignedObjectArray.h>
#include <bullet/LinearMath/btVector3.h>
#include <OGRE/OgreSceneNode.h>
#include "Physics.h"
#include "Trigger.h"
#include "Script/Script.h"

namespace QL
{


// This class is only used as a container for triggers.
class TriggerManager
{
private:
	Physics * m_physics;
	btAlignedObjectArray<Trigger*> m_triggerArray;
	btCollisionShape * m_collisionShape; // Common collision shape for all triggers;

public:
	TriggerManager(Physics * physics);
	~TriggerManager();

	Trigger * createTrigger(const btVector3 & origin, Script * script);
	void addTrigger(Trigger * trigger);
};

}

#endif // QL_TRIGGER_MANAGER_H
