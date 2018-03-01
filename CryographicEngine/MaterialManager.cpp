#include "MaterialManager.h"

MaterialManager* MaterialManager::theInstance = nullptr;

MaterialManager* MaterialManager::GetInstance() {
	if (theInstance == nullptr) {
		theInstance = new MaterialManager();
	}
	return theInstance;
}

MaterialManager::~MaterialManager() {
	materials.clearKeysAndValues();
}

MaterialManager::HandleType MaterialManager::StoreMaterial(std::string &name, Material* material) {
	material->SetName(name);
	return materials.Put(name, material);
}

Material* MaterialManager::GetMaterial(MaterialManager::HandleType &handle) {
	Material *output = materials.Get(handle);
	return output;
}

Material* MaterialManager::GetMaterial(std::string &name) {
	Material *output = nullptr;
	MaterialManager::HandleType handle = materials.Get(name);
	output = GetMaterial(handle);
	return output;
}