#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Object.h"
#include "MeshManager.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "SceneNode.h"
#include "RigidBody.h"
#include <map>

// Taught in Game Engine Design Class @ Humber College

class GameObject : public Object {
private:
	std::string name;
	SceneNode* sceneNode;
	std::string meshName;
	std::string modelName;
	glm::mat4 modelMatrix;
	Light* light;
	BoundingVolume* boundingVolume;
	RigidBody* rigidBody;
	static std::map<std::string, GameObject*> *nameIndex;

	static void RegisterGameObject(std::string &_name, GameObject *object);
	static void DeregisterGameObject(std::string &_name);

public:
	GameObject(std::string& _name);
	GameObject(std::string& _name, Mesh* mesh);
	GameObject(std::string& _name, GameObject* parent, Mesh* mesh);
	GameObject(std::string& _name, Model* model);
	GameObject(std::string& _name, GameObject* parent, Model* model);
	GameObject(std::string& _name, Light* light);
	GameObject(std::string& _name, GameObject* parent, Light* light);

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

	void AttachModel(Model* model);
	void DetachModel();
	bool HasModel();
	Model* GetAttachedModel();

	void AttachLight(Light* light);
	void DetachLight();
	bool HasLight();

	BoundingVolume* GetBoundingVolume();
	RigidBody* GetRigidBody();
	Light* GetAttachedLight();
	void SetModelMatrix(glm::mat4 _model);
	glm::mat4 GetModelMatrix();

	virtual void PreRender();
	virtual void Render(Camera *camera, std::vector<Light*> lights, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	virtual void Render(Shader *shader);
	virtual void PostRender();
	virtual void Update(float deltaTime);
};
#endif
