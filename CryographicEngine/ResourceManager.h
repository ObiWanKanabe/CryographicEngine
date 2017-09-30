#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <vector>

template<class ResourceType>

class ResourceManager {
public:
	ResourceManager() {};
	~ResourceManager() {};

	void addResource() {};

	std::vector<ResourceType*>;
	std::map<std::string name, Resourcetype type> map;
};
#endif

