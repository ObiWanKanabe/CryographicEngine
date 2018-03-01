#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include "ResourceManager.h"
#include "Material.h"

class MaterialManager {
public:
	typedef ResourceHandle<Material> HandleType;
private:

	ResourceManager<Material> materials;
	static MaterialManager* theInstance;

	Material Manager() {};
public:

	static MaterialManager* GetInstance();

	~MaterialManager();

	MaterialManager::HandleType StoreMaterial(std::string &name, Material* _material);

	Material* GetMaterial(MaterialManager::HandleType &handle);

	Material* GetMaterial(std::string &name);
};

#endif

