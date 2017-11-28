#include "MeshManager.h"

MeshManager* MeshManager::theInstance = nullptr;

MeshManager* MeshManager::GetInstance() {
	if (theInstance == nullptr) {
		theInstance = new MeshManager();
	}
	return theInstance;
}

MeshManager::~MeshManager() {
	meshes.clearKeysAndValues();
}

MeshManager::HandleType MeshManager::StoreMesh(std::string &name, Mesh* mesh) {
	return meshes.Put(name, mesh);
}

Mesh* MeshManager::GetMesh(MeshManager::HandleType &handle) {
	Mesh *output = meshes.Get(handle);
	return output;
}

Mesh* MeshManager::GetMesh(std::string &name) {
	Mesh *output = nullptr;
	MeshManager::HandleType handle = meshes.Get(name);
	output = GetMesh(handle);
	return output;
}