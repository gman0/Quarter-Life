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

#ifndef CONVERT_H
#define CONVERT_H

#include <OGRE/Ogre.h>
#include <bullet/btBulletDynamicsCommon.h>

namespace Ogre
{

class DefaultHardwareBufferManager;

}

class btSerializer;


class Convert
{
private:
	Ogre::LogManager * m_logManager;
	Ogre::DefaultHardwareBufferManager * m_hwBufManager;
	Ogre::ResourceGroupManager * m_resourceManager;
	Ogre::LodStrategyManager * m_lodManager;
	Ogre::MeshManager * m_meshManager;
	Ogre::MeshSerializer * m_meshSerializer;
	btDefaultSerializer * m_serializer;
	unsigned char * m_buffer;
	size_t m_bufferSize;


public:
	Convert(bool verbose = false);
	~Convert();

	void convertMeshToBullet(const char * in, const char * out, unsigned flags);

private:
	void initOgre(bool verbose);
	void initBullet();
	bool isOn(unsigned flags, char mask);
};

#endif // CONVERT_H
