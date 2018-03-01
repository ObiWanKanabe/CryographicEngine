#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Object.h"
#include "MeshManager.h"
#include "SceneNode.h"
#include "RigidBody.h"
#include <map>


class GameObject : public Object {
private:
	std::string name;
	SceneNode* sceneNode;
	std::string meshName;
	BoundingVolume* boundingVolume;
	RigidBody* rigidBody;
	static std::map<std::string, GameObject*> *nameIndex;

	static void RegisterGameObject(std::string &_name, GameObject *object);
	static void DeregisterGameObject(std::string &_name);

public:
	GameObject(std::string& _name);
	GameObject(std::string& _name, Mesh* mesh);
	GameObject(std::string& _name, GameObject* parent, Mesh* mesh);

	~GameObject();

	std::string GetName();
	static GameObject* FindGameObject(std::string& _name);
	static std::map<std::string, GameObject*>::iterator begin();
	static std::map<std::string, GameObject*>::iterator end();

	SceneNode* GetSceneNode();

	void AttachToSceneNode(SceneNode* node);
	void DetachFromSceneNode();
	bool IsAttached();

	void AttachChild(GameObject* child);
	void DetachChild(GameObject* child);

	void SetPosition(glm::vec3 _position);
	glm::vec3 GetPosition();
	glm::vec3 GetWorldPosition();

	void SetRotation(glm::vec3 _rotation);
	void SetRotation(glm::quat _rotation);
	glm::quat GetRotation();

	void SetScale(glm::vec3 _scale);
	glm::vec3 GetScale();

	void AttachMesh(Mesh* mesh);
	void DetachMesh();
	bool HasMesh();
	Mesh* GetAttachedMesh();

	BoundingVolume* GetBoundingVolume();
	RigidBody* GetRigidBody();

	virtual void PreRender();
	virtual void Render(Camera *camera, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	virtual void PostRender();
	virtual void Update(float deltaTime);
};
#endif
