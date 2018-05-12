#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <vector>
#include "Hashtable.h"
#include <iostream>

// Taught in Game Engine Design Class @ Humber College

template<class ResourceType>

class ResourceHandle
{
private:
	int		index;
public:
	
	// Default Constructor
	ResourceHandle() : index(-1) {}

	// Constructor requiring the index
	ResourceHandle(int idx) : index(idx) {}

	// Returns null if handle is empty (index < 0)
	bool IsNull() {
		return index < 0;
	}

	// Makes the handle null
	void Nullify() {
		index = -1;
	}

	// Returns the index of the handle
	int GetIndex() {
		return index;
	}

};

template<class ResourceType>

class ResourceManager
{
private:
	std::vector<ResourceType*>								resourceList;
	Hashtable<std::string, ResourceHandle<ResourceType>>	*nameTable;
public:
	
	// Default Constructor makes a hashtable of size 111
	ResourceManager() {
		nameTable = new Hashtable<std::string, ResourceHandle<ResourceType>>(111);
	}

	// Constructor that makes a hashtable of passed in size
	ResourceManager(int sz) {
		nameTable = new Hashtable<std::string, ResourceHandle<ResourceType>>(sz);
	}

	// Deconstructor deletes the hashtable
	~ResourceManager() {
		if (nameTable != NULL) {
			delete nameTable;
			nameTable = NULL;
		}
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
	void remove(std::string &name) {
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
	void clearKeysAndValues() {

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
