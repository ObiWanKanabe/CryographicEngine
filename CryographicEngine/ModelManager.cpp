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
	delete theInstance;
	theInstance = nullptr;
}

Model* ModelManager::LoadModel(std::string &name, std::string filePath) {
	Model* model = new Model(name, filePath);
	StoreModel(name, model);
	return model;
}

Model* ModelManager::LoadModel(std::string &name, std::string filePath, Shader* shader) {
	Model* model = new Model(name, filePath, shader);
	StoreModel(name, model);
	return model;
}

ModelManager::HandleType ModelManager::StoreModel(std::string &name, Model* model) {
	ModelManager::HandleType output(-1);
	output = models.Get(name);
	if (!output.IsNull()) {
		std::cerr << "ERROR : Model name: " << name << " is already in use." << std::endl;
		return output;
	}
	for (size_t i = 0; i < model->GetMeshes().size(); i++) {
		MeshManager::GetInstance()->StoreMesh(model->GetMeshes().at(i)->GetName(), model->GetMeshes().at(i));
		Material* material = MaterialManager::GetInstance()->GetMaterial(model->GetMeshes().at(i)->GetMaterialName());
		material->SetName(material->GetName());
		material->SetShaderName(model->GetShaderName());
	}
	return models.Put(name, model);
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