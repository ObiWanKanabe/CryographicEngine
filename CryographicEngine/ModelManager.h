#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "ResourceManager.h"
#include "Model.h"

class ModelManager {
public:
	typedef ResourceHandle<Model> HandleType;

private:

	ResourceManager<Model> models;
	static ModelManager* theInstance;

	ModelManager() {};
public:

	static ModelManager* GetInstance();

	~ModelManager();

	ModelManager::HandleType StoreModel(std::string &name, Model *model);

	Model* GetModel(ModelManager::HandleType &handle);

	Model* GetModel(std::string &name);
};
#endif
