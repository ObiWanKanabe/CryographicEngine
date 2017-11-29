#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "ResourceManager.h"
#include "Mesh.h"

class MeshManager {
public:
	typedef ResourceHandle<Mesh> HandleType;

private:

	ResourceManager<Mesh> meshes;
	static MeshManager* theInstance;

	MeshManager() {};
public:

	static MeshManager* GetInstance();

	~MeshManager();

	MeshManager::HandleType StoreMesh(std::string &name, Mesh *mesh);

	Mesh* GetMesh(MeshManager::HandleType &handle);

	Mesh* GetMesh(std::string &name);
};
#endif


