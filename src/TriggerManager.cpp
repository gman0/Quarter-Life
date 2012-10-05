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

#include "TriggerManager.h"
#include "Physics.h"

using namespace QL;
using namespace Ogre;

TriggerManager::TriggerManager(Physics * phyiscs) : m_physics(phyiscs)
{
	m_collisionShape = new btSphereShape(1);
}

TriggerManager::~TriggerManager()
{
	for (int i = m_triggerArray.size() - 1; i >= 0; i--)
		delete m_triggerArray[i];
	
	delete m_collisionShape;
}

Trigger * TriggerManager::createTrigger(const btVector3 & origin, Script * script)
{
	Trigger * t = new Trigger(m_physics, m_collisionShape, origin, script);
	addTrigger(t);

	return t;
}

void TriggerManager::addTrigger(Trigger * trigger)
{
	m_triggerArray.push_back(trigger);
}
