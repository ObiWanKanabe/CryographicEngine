#include "GameObject.h"

std::map<std::string, GameObject*> *GameObject::nameIndex = nullptr;

GameObject::GameObject(std::string& _name) {
	name = _name;
	sceneNode = nullptr;
	boundingVolume = nullptr;
	meshName = nullptr;
	if (nameIndex == nullptr) {
		nameIndex = new std::map<std::string, GameObject*>();
	}
	RegisterGameObject(_name, this);
}

GameObject::GameObject(std::string& _name, Mesh *mesh) {
	name = _name;
	sceneNode = new SceneNode();
	sceneNode->AddObject(this);
	boundingVolume = new BoundingVolume(BOUNDING_SHAPE::AABB, mesh);
	AttachMesh(mesh);
	if (nameIndex == nullptr) {
		nameIndex = new std::map<std::string, GameObject*>();
	}
	RegisterGameObject(_name, this);
}

GameObject::GameObject(std::string& _name, GameObject* parent, Mesh* mesh) {
	name = _name;
	if (nameIndex == nullptr)
	{
		nameIndex = new std::map<std::string, GameObject*>();
	}
	RegisterGameObject(name, this);

	parent->AttachChild(this);
	boundingVolume = new BoundingVolume(BOUNDING_SHAPE::AABB, mesh);
	AttachMesh(mesh);
}

GameObject::~GameObject() {
	DetachFromSceneNode();
	DeregisterGameObject(name);
}

void GameObject::RegisterGameObject(std::string& _name, GameObject* object) {
	std::map<std::string, GameObject*>::iterator it = nameIndex->find(_name);
	nameIndex->operator[](_name) = object;
}

void GameObject::DeregisterGameObject(std::string& _name) {
	nameIndex->erase(_name);
}

std::string GameObject::GetName() {
	return name;
}

GameObject* GameObject::FindGameObject(std::string& _name) {
	GameObject* result = nullptr;
	std::map<std::string, GameObject*>::iterator it = nameIndex->find(_name);
	if (it != nameIndex->end())
	{
		result = (*it).second;
	}
	return result;
}

std::map<std::string, GameObject*>::iterator GameObject::begin() {
	return nameIndex->begin();
}

std::map<std::string, GameObject*>::iterator GameObject::end() {
	return nameIndex->end();
}

SceneNode* GameObject::GetSceneNode() {
	return sceneNode;
}

void GameObject::AttachToSceneNode(SceneNode* node) {
	if (IsAttached()) {
		DetachFromSceneNode();
	}
	sceneNode = node;
	sceneNode->AddObject(this);
}

void GameObject::DetachFromSceneNode() {
	if (IsAttached()) {
		sceneNode->RemoveObject(this);
		sceneNode = nullptr;
	}
}

bool GameObject::IsAttached() {
	return sceneNode != nullptr;
}

void GameObject::AttachChild(GameObject* child) {
	sceneNode->AttachChild(child->GetSceneNode());
}

void GameObject::DetachChild(GameObject* child)
{
	SceneNode* childPtr = sceneNode->GetFirstChild();
	while (childPtr != nullptr)
	{
		SceneNode::objectIterator it;
		for (it = childPtr->ObjectBegin(); it != childPtr->ObjectEnd(); it++)
		{
			if ((*it) == child)
			{
				childPtr->RemoveObject(child);
				if (childPtr->GetNumberObjects() == 0)
				{
					sceneNode->RemoveChild(childPtr);
				}
				return;
			}
		}
		childPtr = childPtr->GetNextSibling();
	}
}

void GameObject::SetPosition(glm::vec3 _position) {
	sceneNode->SetPosition(_position);
}

glm::vec3 GameObject::GetPosition() {
	return sceneNode->GetPosition();
}

glm::vec3 GameObject::GetWorldPosition() {
	return sceneNode->GetWorldPosition();
}

void GameObject::SetRotation(glm::vec3 _rotation) {
	sceneNode->SetRotation(_rotation);
}

void GameObject::SetRotation(glm::quat _rotation) {
	sceneNode->SetRotation(_rotation);
}

glm::quat GameObject::GetRotation() {
	return sceneNode->GetRotation();
}

void GameObject::SetScale(glm::vec3 _scale) {
	sceneNode->SetScale(_scale);
}

glm::vec3 GameObject::GetScale() {
	return sceneNode->GetScale();
}

void GameObject::AttachMesh(Mesh* mesh) {
	meshName = mesh->GetName();
}

void GameObject::DetachMesh() {
	meshName = "";
}

bool GameObject::HasMesh() {
	return meshName != "";
}

Mesh* GameObject::GetAttachedMesh() {
	Mesh* mesh;
	mesh = MeshManager::GetInstance()->GetMesh(meshName);
	return mesh;
}

BoundingVolume* GameObject::GetBoundingVolume() {
	return boundingVolume;
}

RigidBody* GameObject::GetRigidBody() {
	return rigidBody;
}

void GameObject::PreRender() {
	Mesh* mesh;
	mesh = MeshManager::GetInstance()->GetMesh(meshName);
	mesh->PreRender();
}

void GameObject::Render(Camera *camera, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
	Mesh* mesh;
	mesh = MeshManager::GetInstance()->GetMesh(meshName);
	mesh->BindUniforms(camera, modelMatrix, viewMatrix, projectionMatrix);
	mesh->Render();
	std::cout << boundingVolume->GetBoundingBox()->c.x << std::endl;
}

void GameObject::PostRender() {
	Mesh* mesh;
	mesh = MeshManager::GetInstance()->GetMesh(meshName);
	mesh->PostRender();
}

void GameObject::Update(float deltaTime) {

}