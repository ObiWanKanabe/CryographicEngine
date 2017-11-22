#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Mesh.h"
#include "ResourceManager.h"
#include "ShaderManager.h"

class GameObject {
public:
	GameObject();
	~GameObject();
	virtual void Update() = 0;
	virtual void Render() = 0;
private:
	ResourceHandle<GameObject> handle;
};
#endif
