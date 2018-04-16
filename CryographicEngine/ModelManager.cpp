#include "ModelManager.h"

ModelManager* ModelManager::theInstance = nullptr;

ModelManager* ModelManager::GetInstance() {
	if (theInstance == nullptr) {
		theInstance = new ModelManager();
	}
	return theInstance;
}

ModelManager::~ModelManager() {
	models.clearKeysAndValues();
}

ModelManager::HandleType ModelManager::StoreModel(std::string &name, Model* Model) {
	ModelManager::HandleType output(-1);
	output = models.Get(name);
	if (!output.IsNull()) {
		std::cerr << "ERROR : Model name: " << name << " is already in use." << std::endl;
		return output;
	}
	Model->SetName(name);
	return models.Put(name, Model);
}

Model* ModelManager::GetModel(ModelManager::HandleType &handle) {
	Model *output = models.Get(handle);
	if (output == nullptr) {
		std::cerr << "ERROR : Model handle is not in use." << std::endl;
	}
	return output;
}

Model* ModelManager::GetModel(std::string &name) {
	ModelManager::HandleType handle = models.Get(name);
	if (handle.IsNull()) {
		std::cerr << "ERROR : Model name: " << name << " is not in use." << std::endl;
	}
	Model *output = GetModel(handle);
	return output;
}