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

#include <iostream>
#include <fstream>
#include <OGRE/OgreDefaultHardwareBufferManager.h>
#include <bullet/LinearMath/btSerializer.h>
#include "Convert.h"
#include "flags.h"
#include "BtOgreGP.h"

using namespace std;
using namespace Ogre;

Convert::Convert(bool verbose)
{
	initOgre(verbose);
	initBullet();
}

Convert::~Convert()
{
	delete m_buffer;
	delete m_serializer;
	delete m_meshSerializer;
	delete m_meshManager;
	delete m_lodManager;
	delete m_resourceManager;
	delete m_hwBufManager;
	delete m_logManager;
}


void Convert::convertMeshToBullet(const char * in, const char * out, unsigned flags)
{
	String meshFileName(in);
	ifstream fileStream(in);

	if (!fileStream.good())
		throw IOException(IOException::ERR_FILE_NOT_FOUND, "Cannot read from \"" + meshFileName + "\"",
							"Convert::convertMeshToBullet", __FILE__, __LINE__);

	fileStream.seekg(0, ios_base::end);
	size_t size = fileStream.tellg();
	fileStream.seekg(0, ios_base::beg);

	MemoryDataStream * memoryStream = new MemoryDataStream(meshFileName, size, true);
	fileStream.readsome((char*)memoryStream->getPtr(), size);
	fileStream.close();

	Mesh mesh(m_meshManager, out, 0, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	DataStreamPtr dataStream(memoryStream);
	m_meshSerializer->importMesh(dataStream, &mesh);

	LogManager::getSingleton().logMessage("Mesh \"" + meshFileName + "\" loaded");


	ofstream outFile(out);

	m_serializer->startSerialization();

	if (mesh.hasSkeleton())
	{
		// dynamic
	}
	else
	{
		// static

		cout << isOn(flags, S_TRIMESH) << " " << flags << " " << S_TRIMESH << endl;

		BtOgre::StaticMeshToShapeConverter converter;
		converter.addMesh(mesh);

		// LogManager::getSingleton().logMessage(
			// "Writing BVH trimesh collision shape into \"" + String(out) + "\"");
		// LogManager::getSingleton().logMessage(
			// "Writing static ConvexHull collision shape into '" + String(out) + "'");
		String type;
		btCollisionShape * colShape;

		if (isOn(flags, S_TRIMESH))
		{
			colShape = converter.createTrimesh();
			type = "BVH trimesh";
		}
		else if (isOn(flags, S_SPHERE))
		{
			colShape = converter.createSphere();
			type = "sphere";
		}
		else if (isOn(flags, S_BOX))
		{
			colShape = converter.createBox();
			type = "box";
		}
		else if (isOn(flags, S_CYLINDER))
		{
			colShape = converter.createCylinder();
			type = "cylinder";
		}
		else if (isOn(flags, S_CONVEX))
		{
			colShape = converter.createConvex();
			type = "convex";
		}

		// colShape = converter.createConvex();
		// colShape = converter.createTrimesh();

		LogManager::getSingleton().logMessage("Writing " + type + " collision shape into '" + out + "'");

		m_serializer->registerNameForPointer(colShape, out);
		colShape->serializeSingleShape(m_serializer);
		// colShape->serialize(m_buffer, m_serializer);
	}

	m_serializer->finishSerialization();
	outFile.write((const char*)m_serializer->getBufferPointer(), m_serializer->getCurrentBufferSize());

	outFile.close();
}

void Convert::initOgre(bool verbose)
{
	m_logManager = new LogManager;
	m_logManager->createLog("stdout log", false, true, true);
	m_hwBufManager = new DefaultHardwareBufferManager;
	m_resourceManager = new ResourceGroupManager;
	m_lodManager = new LodStrategyManager;
	m_meshManager = new MeshManager;
	m_meshSerializer = new MeshSerializer;
}

void Convert::initBullet()
{
	m_bufferSize = 1024 * 1024 * 5;
	// m_buffer = new unsigned char[m_bufferSize];
	m_serializer = new btDefaultSerializer(m_bufferSize);
}

bool Convert::isOn(unsigned flags, char mask)
{
	return (flags & mask) == mask;
}
