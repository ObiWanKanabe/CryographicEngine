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
	delete theInstance;
	theInstance = nullptr;
}

MaterialManager::HandleType MaterialManager::StoreMaterial(std::string &name, Material* material) {
	MaterialManager::HandleType output(-1);
	output = materials.Get(name);
	if (!output.IsNull()) {
		std::cerr << "ERROR : Material name: " << name << " is already in use." << std::endl;
		return output;
	}
	material->SetName(name);
	return materials.Put(name, material);
}

Material* MaterialManager::GetMaterial(MaterialManager::HandleType &handle) {
	Material *output = materials.Get(handle);
	if (output == nullptr) {
		std::cerr << "ERROR : Material handle is not in use." << std::endl;
	}
	return output;
}

Material* MaterialManager::GetMaterial(std::string &name) {
	MaterialManager::HandleType handle = materials.Get(name);
	if (handle.IsNull()) {
		std::cerr << "ERROR : Material name: " << name << " is not in use." << std::endl;
	}
	Material *output = GetMaterial(handle);
	return output;
}