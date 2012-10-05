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

#include <OGRE/Ogre.h>
#include "SceneLoader.h"
#include "ObjectManager.h"
#include "ObjectFileManager.h"
#include "Object.h"
#include <iostream>

using namespace QL;
using namespace Ogre;
using namespace tinyxml2;
using namespace std;

SceneLoader::SceneLoader(ObjectManager * objManager, TriggerManager * triggerManager) :
	m_objectManager(objManager), m_triggerManager(triggerManager), m_attachNode(0)
{
	m_xmlDocument = new XMLDocument;
}

SceneLoader::~SceneLoader()
{
	delete m_xmlDocument;
}

void SceneLoader::parseScene(SceneManager * sceneManager, const String & sceneName, const String & groupName,
							SceneNode * attachNode, const String & prependNode)
{
	m_sceneManager = sceneManager;

	XMLElement * xmlRoot = 0;

	try
	{
		String basename, path;
		StringUtil::splitFilename(sceneName, basename, path);

		DataStreamPtr stream = ResourceGroupManager::getSingleton().openResource(basename, groupName);

		String data = stream->getAsString();
		stream->close();
		stream.setNull();

		m_xmlDocument->Parse(data.c_str());

		LogManager::getSingleton().logMessage("SceneLoader: Loading scene '" + basename + "'");

		if (m_xmlDocument->Error())
		{
			String errMsg = "SceneLoader: tinyxml2 reported an error: ";
			errMsg += m_xmlDocument->GetErrorStr1() + String(", ") + m_xmlDocument->GetErrorStr2();

			LogManager::getSingleton().logMessage(errMsg);

			return;
		}
	}
	catch (Exception & e)
	{
		LogManager::getSingleton().logMessage("SceneLoader: Error creating XMLDocument: "
												+ e.getFullDescription());
		return;
	}

	xmlRoot = m_xmlDocument->RootElement();

	if (String(xmlRoot->Name()) != "scene")
	{
		LogManager::getSingleton().logMessage("SceneLoader: Invalid .scene file. Missing <scene> tag.");
		return;
	}

	m_attachNode = (attachNode) ? attachNode : m_sceneManager->getRootSceneNode();

	processScene(xmlRoot);
}

void SceneLoader::processScene(const XMLNode * xmlRootNode)
{
	const XMLNode * node;

	node = xmlRootNode->FirstChildElement("nodes");
	if (node)
		processNodes(node);
}

void SceneLoader::processNodes(const XMLNode * xmlNode)
{
	const XMLElement * elem = xmlNode->FirstChildElement("node");

	while (elem)
	{
		processNode(elem);
		elem = elem->NextSiblingElement("node");
	}
}

void SceneLoader::processNode(const XMLElement * xmlElement, SceneNode * parentNode)
{
	String name = xmlElement->Attribute("name");
	SceneNode * node;

	const XMLElement * elem;

	if (name == "_start")
	{
		// TODO: starting camera orientation
		elem = xmlElement->FirstChildElement("position");
		m_start = parseVector3(elem);

		return;
	}

	if (name.empty())
	{
		if (parentNode)
			node = parentNode->createChildSceneNode();
		else
			node = m_attachNode->createChildSceneNode();
	}
	else
	{
		if (parentNode)
			node = parentNode->createChildSceneNode(name);
		else
			node = m_attachNode->createChildSceneNode(name);
	}


	elem = xmlElement->FirstChildElement("position");
	if (elem)
	{
		node->setPosition(parseVector3(elem));
		node->setInitialState();
	}

	elem = xmlElement->FirstChildElement("rotation");
	if (elem)
	{
		node->setOrientation(parseQuaternion(elem));
		node->setInitialState();
	}

	elem = xmlElement->FirstChildElement("scale");
	if (elem)
	{
		node->setScale(parseVector3(elem));
		node->setInitialState();
	}

	// Also process this node's child nodes (if any).
	elem = xmlElement->FirstChildElement("node");
	while (elem)
	{
		processNode(elem, node);
		elem = elem->NextSiblingElement("node");
	}

	elem = xmlElement->FirstChildElement("entity");
	while (elem)
	{
		processEntity(elem, node);
		elem = elem->NextSiblingElement("entity");
	}

	elem = xmlElement->FirstChildElement("light");
	while (elem)
	{
		processLight(elem, node);
		elem = elem->NextSiblingElement("light");
	}

	// TODO: implement userDataReference

	if (checkType(name.c_str(), "_trigger"))
	{
		processTrigger(xmlElement, node);
		return;
	}
}

void SceneLoader::processEntity(const XMLElement * xmlElement, SceneNode * parentNode)
{
	String name = getAttribute(xmlElement, "name");
	String objFile = getAttribute(xmlElement, "objectFile");
	String materialFile = getAttribute(xmlElement, "materialFile");
	bool castShadows = getAttributeBool(xmlElement, "castShadows", true);
	String physType = getAttribute(xmlElement, "physics_type");
	Real mass;

	if (physType == "STATIC")
		mass = 0;
	else
		mass = getAttributeReal(xmlElement, "mass");

	try
	{
		ObjectFilePtr objFilePtr = ObjectFileManager::getSingleton().load(objFile,
										ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
		Object * obj = m_objectManager->addObject(objFilePtr, parentNode, mass);

		Entity * entity = obj->entity;
		entity->setCastShadows(castShadows);

		if (!materialFile.empty())
			entity->setMaterialName(materialFile);
	}
	catch (Exception & e)
	{
		LogManager::getSingleton().logMessage("SceneLoader: Cannot create object: " + e.getFullDescription());
	}
}

void SceneLoader::processLight(const XMLElement * xmlElement, SceneNode * parentNode)
{
	String name = getAttribute(xmlElement, "name");
	Light * light = m_sceneManager->createLight(name);

	if (parentNode)
		parentNode->attachObject(light);
	
	String type = getAttribute(xmlElement, "type");

	if (type == "point")
		light->setType(Light::LT_POINT);
	else if (type == "directional" || type == "radPoint")
		light->setType(Light::LT_DIRECTIONAL);
	else if (type == "spot")
		light->setType(Light::LT_SPOTLIGHT);

	light->setVisible(getAttributeBool(xmlElement, "visible", true));
	light->setCastShadows(getAttributeBool(xmlElement, "castShadows", true));

	const XMLElement * elem;

	elem = xmlElement->FirstChildElement("position");
	if (elem)
		light->setPosition(parseVector3(elem));
	
	elem = xmlElement->FirstChildElement("normal");
	if (elem)
		light->setDirection(parseVector3(elem));
	
	elem = xmlElement->FirstChildElement("colourDiffuse");
	if (elem)
		light->setDiffuseColour(parseColour(elem));
	
	elem = xmlElement->FirstChildElement("colourSpecular");
	if (elem)
		light->setSpecularColour(parseColour(elem));
	
	elem = xmlElement->FirstChildElement("lightRange");
	if (elem)
		processLightRange(light, xmlElement);
	
	elem = xmlElement->FirstChildElement("lightAttenuation");
	if (elem)
		processLightAttenuation(light, xmlElement);
}

void SceneLoader::processTrigger(const XMLElement * xmlElement, SceneNode * parentNode)
{
	Properties props = getProperties(xmlElement);
	String objFile = props["script"];

	if (!objFile.empty())
	{
		try
		{
			ObjectFilePtr objFilePtr = ObjectFileManager::getSingleton().load(objFile,
											ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
			m_objectManager->addObject(objFilePtr, parentNode, 0);
		}
		catch (Exception & e)
		{
			LogManager::getSingleton().logMessage(
				"SceneLoader: Cannot create object: " + e.getFullDescription());
		}
	}
}

void SceneLoader::processLightAttenuation(Light * light, const XMLElement * xmlElement)
{
	Real range = getAttributeReal(xmlElement, "range");
	Real constant = getAttributeReal(xmlElement, "constant");
	Real linear = getAttributeReal(xmlElement, "linear");
	Real quadratic = getAttributeReal(xmlElement, "quadratic");

	light->setAttenuation(range, constant, linear, quadratic);
}

void SceneLoader::processLightRange(Light * light, const XMLElement * xmlElement)
{
	Real inner = getAttributeReal(xmlElement, "inner");
	Real outer = getAttributeReal(xmlElement, "outer");
	Real falloff = getAttributeReal(xmlElement, "falloff", 1.0);

	light->setSpotlightRange(Angle(inner), Angle(outer), falloff);
}

String SceneLoader::getAttribute(const XMLElement * xmlElement, const String & param,
								const String & defaultValue)
{
	const char * attr = xmlElement->Attribute(param.c_str());

	if (attr)
		return attr;
	else
		return defaultValue;
}

Real SceneLoader::getAttributeReal(const XMLElement * xmlElement, const String & param,
									Real defaultValue)
{
	Real num = defaultValue;

#if OGRE_DOUBLE_PRECISION == 1
	xmlElement->QueryDoubleAttribute(param.c_str(), &num);
#else
	xmlElement->QueryFloatAttribute(param.c_str(), &num);
#endif

	return num;
}

bool SceneLoader::getAttributeBool(const XMLElement * xmlElement, const String & param,
									bool defaultValue)
{
	bool ret = defaultValue;
	xmlElement->QueryBoolAttribute(param.c_str(), &ret);

	return ret;
}

Vector3 SceneLoader::parseVector3(const XMLElement * xmlElement)
{
	return Vector3(
		StringConverter::parseReal(xmlElement->Attribute("x")),
		StringConverter::parseReal(xmlElement->Attribute("y")),
		StringConverter::parseReal(xmlElement->Attribute("z"))
	);
}

Quaternion SceneLoader::parseQuaternion(const XMLElement * xmlElement)
{
	return Quaternion(
		StringConverter::parseReal(xmlElement->Attribute("qw")),
		StringConverter::parseReal(xmlElement->Attribute("qx")),
		StringConverter::parseReal(xmlElement->Attribute("qy")),
		StringConverter::parseReal(xmlElement->Attribute("qz"))
	);
}

ColourValue SceneLoader::parseColour(const XMLElement * xmlElement)
{
	return ColourValue(
		StringConverter::parseReal(xmlElement->Attribute("r")),
		StringConverter::parseReal(xmlElement->Attribute("g")),
		StringConverter::parseReal(xmlElement->Attribute("b")),
		(xmlElement->Attribute("a")) ? StringConverter::parseReal(xmlElement->Attribute("a")) : 1
	);
}

bool SceneLoader::checkType(const char * name, const char * type)
{
	const char * separator = strrchr(name, '_');

	if (separator)
		return !(strcmp(separator, type));
	else
		return false;
}

const Vector3 & SceneLoader::getStart() const
{
	return m_start;
}

SceneLoader::Properties SceneLoader::getProperties(const XMLElement * xmlElement)
{
	Properties props;
	const XMLElement * elem = xmlElement->FirstChildElement("property");

	while (elem)
	{
		props[getAttribute(elem, "name")] = getAttribute(elem, "value");
		elem = elem->NextSiblingElement("property");
	}

	return props;
}

Ogre::String SceneLoader::getProperty(SceneLoader::Properties & props, const String & name)
{
	return props[name];
}
