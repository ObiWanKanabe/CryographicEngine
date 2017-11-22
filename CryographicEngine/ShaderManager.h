#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <string>
#include <vector>
#include "Hashtable.h"
#include <iostream>
#include "Shader.h"

template<class ResourceType>

class ShaderManager {
private:
	std::vector<ResourceType*>								resourceList;
	Hashtable<std::string, ResourceHandle<ResourceType>>	*nameTable;
public:

	// Default Constructor makes a hashtable of size 111
	ShaderManager() {
		nameTable = new Hashtable<std::string, ResourceHandle<ResourceType>>(111);
	}

	// Constructor that makes a hashtable of passed in size
	ShaderManager(int sz) {
		nameTable = new Hashtable<std::string, ResourceHandle<ResourceType>>(sz);
	}

	// Deconstructor deletes the hashtable
	~ShaderManager() {
		if (nameTable != NULL) {
			delete nameTable;
			nameTable = NULL;
		}
	}

	virtual ResourceHandle<ResourceType> StoreShader(std::string &name, const char* vertexPath, const char* fragmentPath) {
		Shader *shader = new Shader(vertexPath, fragmentPath);
		return Put(name, shader);
	}

	// Puts a resource with the given name into the resource manager
	virtual ResourceHandle<ResourceType> Put(std::string &name, ResourceType *res) {
		Hashtable<std::string, ResourceHandle<ResourceType>>::iterator	iter;
		int key = Hashtable<std::string, ResourceHandle<ResourceType>>::stringhash(name);

		iter = nameTable->get(key);
		if (iter == nameTable->end()) {
			int idx = resourceList.size();
			resourceList.push_back(res);
			ResourceHandle<ResourceType> handle(idx);
			HashItem<std::string, ResourceHandle<ResourceType>> item(key, name, handle);
			nameTable->put(item);
			return handle;
		}
		return (*iter).getValue();
	}

	// Gets the resource from the list using the given handle
	ResourceType* Get(ResourceHandle<ResourceType> &handle) const {
		int idx = handle.GetIndex();
		ResourceType *result = NULL;

		if (idx >= 0 && idx < int(resourceList.size())) {
			result = resourceList[idx];
		}
		return result;
	}

	// Returns the resource list
	ResourceHandle<ResourceType> Get(const std::string &name) const {
		Hashtable<std::string, ResourceHandle<ResourceType>>::iterator	iter;
		int key = Hashtable<std::string, ResourceHandle<ResourceType>>::stringhash((std::string)name);

		iter = nameTable->get(key);
		if (iter == nameTable->end())
		{
			return ResourceHandle<ResourceType>();
		}
		return (*iter).getValue();
	}

	// Removes the resource with the given name
	void Remove(std::string &name) {
		Hashtable<std::string, ResourceHandle<ResourceType>>::iterator	iter;
		int key = Hashtable<std::string, ResourceHandle<ResourceType>>::stringhash(name);

		iter = nameTable->get(key);
		if (iter != nameTable->end()) {
			int idx = (*iter).getValue().getIndex();
			resourceList[idx] = NULL;
			nameTable->remove(key);
		}
	}

	//Clears the resource manager
	void ClearKeysAndValues() {

		std::vector<int>	deleteKeyList;

		Hashtable<std::string, ResourceHandle<ResourceType> >::iterator	iter = nameTable->begin();

		while (iter != nameTable->end()) {
			HashItem<std::string, ResourceHandle<ResourceType> > item = *iter;
			ResourceHandle<ResourceType> handle = item.getValue();
			if (!handle.IsNull()) {
				ResourceType *res = resourceList[handle.GetIndex()];
				if (res != NULL) {
					delete res;
					resourceList[handle.GetIndex()] = NULL;
				}
				deleteKeyList.push_back(item.getKey());
			}
			iter++;
		}

		std::vector<int>::iterator viter = deleteKeyList.begin();
		while (viter != deleteKeyList.end()) {
			int ke = *viter;
			nameTable->remove(ke);
			viter++;
		}

		resourceList.clear();
	}

};
#endif

