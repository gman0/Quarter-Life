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



#ifndef QL_SCENE_LOADER_H
#define QL_SCENE_LOADER_H

#include <vector>
#include <map>
#include <tinyxml2.h>
#include <OGRE/OgreString.h>
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreQuaternion.h>
#include "ObjectManager.h"
#include "TriggerManager.h"


namespace QL
{

class SceneLoader
{
private:
	ObjectManager * m_objectManager;
	TriggerManager * m_triggerManager;

	Ogre::SceneManager * m_sceneManager;
	Ogre::SceneNode * m_attachNode;
	Ogre::Vector3 m_start;

	tinyxml2::XMLDocument * m_xmlDocument;

	typedef std::map<Ogre::String, Ogre::String> Properties;

public:	
	SceneLoader(ObjectManager * objectManager, TriggerManager * triggerManager);
	~SceneLoader();

	void parseScene(Ogre::SceneManager * sceneManager, const Ogre::String & sceneName,
					const Ogre::String & groupName, Ogre::SceneNode * attachNode = 0,
					const Ogre::String & prependNode = "");

	const Ogre::Vector3 & getStart() const;

private:
	void processScene(const tinyxml2::XMLNode * xmlRootNode);

	void processNodes(const tinyxml2::XMLNode * xmlNode);
	void processNode(const tinyxml2::XMLElement * xmlElement, Ogre::SceneNode * parentNode = 0);
	void processEntity(const tinyxml2::XMLElement * xmlElement, Ogre::SceneNode * parentNode);
	void processLight(const tinyxml2::XMLElement * xmlElement, Ogre::SceneNode * parentNode);

	void processTrigger(const tinyxml2::XMLElement * xmlElement, Ogre::SceneNode * parentNode);


	void processLightAttenuation(Ogre::Light * light, const tinyxml2::XMLElement * xmlElement);
	void processLightRange(Ogre::Light * light, const tinyxml2::XMLElement * xmlElement);

	// helpers
	Ogre::String getAttribute(const tinyxml2::XMLElement * xmlNode, const Ogre::String & param,
								const Ogre::String & defaultValue = "");
	Ogre::Real getAttributeReal(const tinyxml2::XMLElement * xmlNode, const Ogre::String & param,
								Ogre::Real defaultValue = 0);
	bool getAttributeBool(const tinyxml2::XMLElement * xmlNode, const Ogre::String & param,
							bool defaultValue = false);
	Ogre::Vector3 parseVector3(const tinyxml2::XMLElement * xmlNode);
	Ogre::Quaternion parseQuaternion(const tinyxml2::XMLElement * xmlElement);
	Ogre::ColourValue parseColour(const tinyxml2::XMLElement * xmlElement);

	/*
	 * Checks type of a scene object which is defined in it's name like so:
	 * {OBJECT_NAME}_{TYPE_NAME}
	 */
	bool checkType(const char * name, const char * type);

	Properties getProperties(const tinyxml2::XMLElement * xmlElement);
	Ogre::String getProperty(Properties & props, const Ogre::String & name);
};

} // namespace QL


#endif // QL_SCENE_LOADER_H
