#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <vector>
#include "Hashtable.h"
#include <iostream>


template<class ResourceType>
class ResourceHandle
{
private:
	int		index;
public:
	/*
	* Parameterless constructor which creates a NULL resource handle.
	*/
	ResourceHandle() :
		index(-1) {}

	/*
	* Constructor which creates a handle to reference the indicated index.
	* @param idx The index of the resource in the ResourceManager.
	*/
	ResourceHandle(int idx) :
		index(idx) {}

	/**
	* Determines if this is a NULL handle which references nothing.
	* @returns true if this is a NULL handle
	*/
	bool isNull() const
	{
		return index < 0;
	}

	/**
	* Change this handle into a NULL handle which references nothing.
	*/
	void nullify()
	{
		index = -1;
	}

	/**
	* Return the current index value.
	* @returs the current value of the handle index.
	*/
	int getIndex() const
	{
		return index;
	}

	/**
	* The assigment operator.
	*/
	ResourceHandle& operator=(ResourceHandle& other)
	{
		index = other.index;
		return *this;
	}

	/**
	* Equality operator.
	*/
	bool operator==(ResourceHandle& other) const
	{
		return index == other.index;
	}

	/**
	* Inequality operator.
	*/
	bool operator!=(ResourceHandle& other) const
	{
		return index != other.index;
	}

	/**
	* less than operator
	*/
	bool operator<(ResourceHandle& other) const
	{
		return index < other.index;
	}

	friend std::ostream& operator<<(std::ostream& os, ResourceHandle &hndl);
};

/**
* The ResourceManager is an in-memory container which stores resources.  The ResourceManager
* is a template class so that each resource type must have its own instance of the
* ResourceManager.  The resources are stored by pointer in a vector which can be accessed by
* the index contained in a ResourceHandle.  There is a secondary index which uses a Hashtable
* to provide rapid access to resources by name when a ResourceHandle is not available.
* @author R. N. Robson
*/
template<class ResourceType>
class ResourceManager
{
private:
	std::vector<ResourceType*>								resourceList;
	Hashtable<std::string, ResourceHandle<ResourceType>>	*nameTable;
public:
	/**
	* Create an empty ResourceManager with a default hashtable size.
	*/
	ResourceManager(void)
	{
		nameTable = new Hashtable<std::string, ResourceHandle<ResourceType>>(111);
	}

	/**
	* Create an empty ResourceManager with a  hashtable of the indicated size.
	* @param sz The size of the hashtable for the name index which should be a prime
	* about 3 times the anticipated number of entries.
	*/
	ResourceManager(int sz)
	{
		nameTable = new Hashtable<std::string, ResourceHandle<ResourceType>>(sz);
	}

	/**
	* Destructor.  This will not delete any of the resources stored in the ResourceManager.
	*/
	~ResourceManager(void)
	{
		if (nameTable != NULL)
		{
			delete nameTable;
			nameTable = NULL;
		}
	}

	/**
	* Add a resource to the ResourceManager.  If the resource is already in the ResourceManager
	* it is neither added again nor does it overwrite the existing entry.
	* @param name The unique name of the resource.
	* @param res A pointer to the resource to add.
	* @returns A handle for the resource which was added.
	*/
	ResourceHandle<ResourceType> put(std::string &name, ResourceType *res)
	{
		Hashtable<std::string, ResourceHandle<ResourceType>>::iterator	iter;
		int key = Hashtable<std::string, ResourceHandle<ResourceType>>::stringhash(name);

		iter = nameTable->get(key);
		if (iter == nameTable->end())
		{
			int idx = resourceList.size();
			resourceList.push_back(res);
			ResourceHandle<ResourceType> handle(idx);
			HashItem<std::string, ResourceHandle<ResourceType>> item(key, name, handle);
			nameTable->put(item);
			return handle;
		}
		return (*iter).getValue();
	}

	/**
	* Return a pointer to the resource represented by the ResourceHandle passed.  If the
	* ResourceHandle is invalid, then NULL is returned.
	* @param handle The handle of the resource to retrieve.
	* @returns A pointer to the resource or NULL if not found.
	*/
	ResourceType* get(ResourceHandle<ResourceType> &handle) const
	{
		int idx = handle.getIndex();
		ResourceType *result = NULL;

		if (idx >= 0 && idx < int(resourceList.size()))
		{
			result = resourceList[idx];
		}
		return result;
	}

	/**
	* This looks up a resource by name and returns a handle for the resource.  If the
	* resource name cannot be found, then a null handle will be returned.
	* @param name The name of the resource to locate.
	* @returns A handle for the resource or a null handle if the resource was not found.
	*/
	ResourceHandle<ResourceType> get(const std::string &name) const
	{
		Hashtable<std::string, ResourceHandle<ResourceType>>::iterator	iter;
		int key = Hashtable<std::string, ResourceHandle<ResourceType>>::stringhash((std::string)name);

		iter = nameTable->get(key);
		if (iter == nameTable->end())
		{
			return ResourceHandle<ResourceType>();
		}
		return (*iter).getValue();
	}

	/**
	* Remove the named resource from the ResourceManager.
	* @param name The name of the resource to remove.
	*/
	void remove(std::string &name)
	{
		Hashtable<std::string, ResourceHandle<ResourceType>>::iterator	iter;
		int key = Hashtable<std::string, ResourceHandle<ResourceType>>::stringhash(name);

		iter = nameTable->get(key);
		if (iter != nameTable->end())
		{
			int idx = (*iter).getValue().getIndex();
			resourceList[idx] = NULL;
			nameTable->remove(key);
		}
	}

	/**
	* This will remove all keys and values from the resource manager.
	* The values stored in the resource manager will be deleted by this operation.
	*/
	void clearKeysAndValues()
	{
		std::vector<int>	deleteKeyList;
		Hashtable<std::string, ResourceHandle<ResourceType> >::iterator	iter = nameTable->begin();

		/////////////////////////////////////////////////////
		// First, we traverse the hashtable and delete the models.
		// We save the string keys to be deleted and delete these
		// on a second pass so we do not mess with the iterator.
		/////////////////////////////////////////////////////
		while (iter != nameTable->end())
		{
			HashItem<std::string, ResourceHandle<ResourceType> > item = *iter;
			ResourceHandle<ResourceType> handle = item.getValue();
			if (!handle.isNull())
			{
				ResourceType *res = resourceList[handle.getIndex()];
				if (res != NULL)
				{
					delete res;
					resourceList[handle.getIndex()] = NULL;
				}
				deleteKeyList.push_back(item.getKey());
			}
			iter++;
		}

		std::vector<int>::iterator viter = deleteKeyList.begin();
		while (viter != deleteKeyList.end())
		{
			int ke = *viter;
			nameTable->remove(ke);
			viter++;
		}

		////////////////////////////////////////////////////////
		// Now, clear the resource list so it does not grow forever
		////////////////////////////////////////////////////////
		resourceList.clear();
	}

	void realityCheck(std::string& msg)
	{
		nameTable->realityCheck(msg);
	}

};

template<class ResourceType>
std::ostream& operator<<(std::ostream& os, ResourceHandle<ResourceType> &hndl)
{
	os << hndl.index;
	return os;
}



#endif
