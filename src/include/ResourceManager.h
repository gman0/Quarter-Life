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



#ifndef QL_RESOURCE_MANAGER_H
#define QL_RESOURCE_MANAGER_H

#include "ObjectFileManager.h"


namespace QL
{


class ResourceManager
{
private:
	const char * m_resourceConfigFileName;
	ObjectFileManager * m_objFileManager;

public:
	ResourceManager(const char * resConfigFileName);
	~ResourceManager();

	void initialiseResources();

private:
	void registerResourceTypes();
	void readResConfigFile();
};


} // namespace QL


#endif // QL_RESOURCE_MANAGER_H
