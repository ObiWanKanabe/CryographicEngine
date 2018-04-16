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
	MeshManager::HandleType output(-1);
	output = meshes.Get(name);
	if (!output.IsNull()) {
		std::cerr << "ERROR : Mesh name: " << name << " is already in use." << std::endl;
		return output;
	}
	mesh->SetName(name);
	return meshes.Put(name, mesh);
}

Mesh* MeshManager::GetMesh(MeshManager::HandleType &handle) {
	Mesh *output = meshes.Get(handle);
	if (output == nullptr) {
		std::cerr << "ERROR : Mesh handle is not in use." << std::endl;
	}
	return output;
}

Mesh* MeshManager::GetMesh(std::string &name) {
	MeshManager::HandleType handle = meshes.Get(name);
	if (handle.IsNull()) {
		std::cerr << "ERROR : Mesh name: " << name << " is not in use." << std::endl;
	}
	Mesh *output = GetMesh(handle);
	return output;
}