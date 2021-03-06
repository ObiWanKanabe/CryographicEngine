#include "GameObject.h"

std::map<std::string, GameObject*> *GameObject::nameIndex = nullptr;

GameObject::GameObject(std::string& _name) {
	name = _name;
	sceneNode = new SceneNode();
	sceneNode->AddObject(this);
	boundingVolume = nullptr;
	DetachMesh();
	DetachModel();
	LOD = nullptr;
	if (nameIndex == nullptr) {
		nameIndex = new std::map<std::string, GameObject*>();
	}
	RegisterGameObject(_name, this);
	light = nullptr;
	environmentMap = nullptr;
	castShadows = true;
	highQualityShadows = false;
}

GameObject::GameObject(std::string& _name, Mesh *mesh) {
	name = _name;
	sceneNode = new SceneNode();
	sceneNode->AddObject(this);
	boundingVolume = new BoundingVolume(BOUNDING_SHAPE::AABB, mesh);
	AttachMesh(mesh);
	LOD = nullptr;
	if (nameIndex == nullptr) {
		nameIndex = new std::map<std::string, GameObject*>();
	}
	RegisterGameObject(_name, this);
	light = nullptr;
	environmentMap = nullptr;
	castShadows = true;
	highQualityShadows = false;
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
	LOD = nullptr;
	light = nullptr;
	environmentMap = nullptr;
	castShadows = true;
	highQualityShadows = false;
}

GameObject::GameObject(std::string& _name, Model *model) {
	name = _name;
	sceneNode = new SceneNode();
	sceneNode->AddObject(this);
	boundingVolume = new BoundingVolume(BOUNDING_SHAPE::AABB, model);
	AttachModel(model);
	LOD = nullptr;
	if (nameIndex == nullptr) {
		nameIndex = new std::map<std::string, GameObject*>();
	}
	RegisterGameObject(_name, this);
	light = nullptr;
	environmentMap = nullptr;
	castShadows = true;
	highQualityShadows = false;
}

GameObject::GameObject(std::string& _name, GameObject* parent, Model* model) {
	name = _name;
	if (nameIndex == nullptr)
	{
		nameIndex = new std::map<std::string, GameObject*>();
	}
	RegisterGameObject(name, this);

	parent->AttachChild(this);
	boundingVolume = new BoundingVolume(BOUNDING_SHAPE::AABB, model);
	AttachModel(model);
	LOD = new LevelOfDetail(model);
	light = nullptr;
	environmentMap = nullptr;
	castShadows = true;
	highQualityShadows = false;
}

GameObject::GameObject(std::string& _name, LevelOfDetail* LOD) {
	name = _name;
	sceneNode = new SceneNode();
	sceneNode->AddObject(this);
	this->LOD = LOD;
	if (this->LOD->GetCurrentLODType(0.0f) == MESH_LOD) {
		Mesh* mesh = MeshManager::GetInstance()->GetMesh(this->LOD->GetCurrentLODName(0.0f));
		boundingVolume = new BoundingVolume(BOUNDING_SHAPE::AABB, mesh);
	} else if (this->LOD->GetCurrentLODType(0.0f) == MODEL_LOD) {
		Model* model = ModelManager::GetInstance()->GetModel(this->LOD->GetCurrentLODName(0.0f));
		boundingVolume = new BoundingVolume(BOUNDING_SHAPE::AABB, model);
	}
	DetachMesh();
	DetachModel();
	if (nameIndex == nullptr) {
		nameIndex = new std::map<std::string, GameObject*>();
	}
	RegisterGameObject(_name, this);
	light = nullptr;
	environmentMap = nullptr;
	castShadows = true;
	highQualityShadows = false;
}

GameObject::GameObject(std::string& _name, GameObject* parent, LevelOfDetail* LOD) {
	name = _name;
	if (nameIndex == nullptr)
	{
		nameIndex = new std::map<std::string, GameObject*>();
	}
	RegisterGameObject(name, this);

	parent->AttachChild(this);
	this->LOD = LOD;
	if (this->LOD->GetCurrentLODType(0.0f) == MESH_LOD) {
		Mesh* mesh = MeshManager::GetInstance()->GetMesh(this->LOD->GetCurrentLODName(0.0f));
		boundingVolume = new BoundingVolume(BOUNDING_SHAPE::AABB, mesh);
	}
	else if (this->LOD->GetCurrentLODType(0.0f) == MODEL_LOD) {
		Model* model = ModelManager::GetInstance()->GetModel(this->LOD->GetCurrentLODName(0.0f));
		boundingVolume = new BoundingVolume(BOUNDING_SHAPE::AABB, model);
	}
	DetachMesh();
	DetachModel();
	light = nullptr;
	environmentMap = nullptr;
	castShadows = true;
	highQualityShadows = false;
}

GameObject::GameObject(std::string& _name, Light* light) {
	name = _name;
	sceneNode = new SceneNode();
	sceneNode->AddObject(this);
	AttachLight(light);
	LOD = nullptr;
	if (nameIndex == nullptr) {
		nameIndex = new std::map<std::string, GameObject*>();
	}
	RegisterGameObject(_name, this);
	environmentMap = nullptr;
	castShadows = true;
	highQualityShadows = false;
}

GameObject::GameObject(std::string& _name, GameObject* parent, Light* light) {
	name = _name;
	if (nameIndex == nullptr)
	{
		nameIndex = new std::map<std::string, GameObject*>();
	}
	RegisterGameObject(name, this);

	parent->AttachChild(this);
	AttachLight(light);
	LOD = nullptr;
	environmentMap = nullptr;
	castShadows = true;
	highQualityShadows = false;
}

GameObject::~GameObject() {
	DetachFromSceneNode();
	DeregisterGameObject(name);
	delete light;
	delete boundingVolume;
	delete rigidBody;
	delete LOD;
	light = nullptr;
	boundingVolume = nullptr;
	rigidBody = nullptr;
	LOD = nullptr;
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
	if (light != nullptr) {
		light->SetPosition(_position);
	}
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
	modelName == "";
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

void GameObject::AttachModel(Model* model) {
	modelName = model->GetName();
	meshName == "";
}

void GameObject::DetachModel() {
	modelName = "";
}

bool GameObject::HasModel() {
	return modelName != "";
}

Model* GameObject::GetAttachedModel() {
	Model* model;
	model = ModelManager::GetInstance()->GetModel(modelName);
	return model;
}

void GameObject::AttachLight(Light* light) {
	this->light = light;
	this->light->SetPosition(sceneNode->GetPosition());
}

void GameObject::DetachLight() {
	this->light = nullptr;
}

bool GameObject::HasLight() {
	return light != nullptr;
}

void GameObject::SetCastShadows(bool _shadows) {
	castShadows = _shadows;
}

bool GameObject::CanCastShadows() {
	return castShadows;
}

void GameObject::HighQualityShadows(bool _bool) {
	highQualityShadows = _bool;
}

void GameObject::EnableEnvironmentMap() {
	environmentMap = new EnvironmentMap(128, GetPosition());
}

void GameObject::DisableEnvironmentMap() {
	environmentMap = nullptr;
}

BoundingVolume* GameObject::GetBoundingVolume() {
	return boundingVolume;
}

RigidBody* GameObject::GetRigidBody() {
	return rigidBody;
}

Light* GameObject::GetAttachedLight() {
	return light;
}

EnvironmentMap* GameObject::GetEnvironmentMap() {
	return environmentMap;
}

void GameObject::SetModelMatrix(glm::mat4 _model) {
	modelMatrix = _model;
}

glm::mat4 GameObject::GetModelMatrix() {
	return modelMatrix;
}

void GameObject::PreRender() {
	if (meshName != "") {
		Mesh* mesh;
		mesh = MeshManager::GetInstance()->GetMesh(meshName);
		mesh->PreRender();
	}
	else if (modelName != "") {
		Model* model;
		model = ModelManager::GetInstance()->GetModel(modelName);
		model->PreRender();
	}
}

void GameObject::Render(Camera *camera, std::vector<Light*> lights, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
	if (meshName != "") {
		Mesh* mesh;
		mesh = MeshManager::GetInstance()->GetMesh(meshName);
		mesh->BindUniforms(camera, lights, modelMatrix, viewMatrix, projectionMatrix);
		mesh->Render();
	}
	else if (modelName != "") {
		Model* model;
		model = ModelManager::GetInstance()->GetModel(modelName);
		model->BindUniforms(camera, lights, modelMatrix, viewMatrix, projectionMatrix);
		model->Render();
	} else if (LOD != nullptr) {

		float distance = glm::length(sceneNode->GetPosition() - camera->GetPosition());

		if (LOD->GetCurrentLODType(distance) == MESH_LOD) {
			Mesh* mesh;
			mesh = MeshManager::GetInstance()->GetMesh(LOD->GetCurrentLODName(distance));
			mesh->BindUniforms(camera, lights, modelMatrix, viewMatrix, projectionMatrix);
			mesh->Render();
		}
		else if (LOD->GetCurrentLODType(distance) == MODEL_LOD) {
			Model* model;
			model = ModelManager::GetInstance()->GetModel(LOD->GetCurrentLODName(distance));
			model->BindUniforms(camera, lights, modelMatrix, viewMatrix, projectionMatrix);
			model->Render();
		}
	}
}

void GameObject::RenderLowDetail(Camera *camera, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
	if (meshName != "") {
		Mesh* mesh;
		mesh = MeshManager::GetInstance()->GetMesh(meshName);
		mesh->BindUniformsLowDetail(camera, modelMatrix, viewMatrix, projectionMatrix);
		mesh->RenderLowDetail();
	}
	else if (modelName != "") {
		Model* model;
		model = ModelManager::GetInstance()->GetModel(modelName);
		model->BindUniformsLowDetail(camera, modelMatrix, viewMatrix, projectionMatrix);
		model->RenderLowDetail();
	} else if (LOD != nullptr) {

		float distance = glm::length(sceneNode->GetPosition() - camera->GetPosition());

		if (LOD->GetCurrentLODType(distance) == MESH_LOD) {
			Mesh* mesh;
			mesh = MeshManager::GetInstance()->GetMesh(LOD->GetCurrentLODName(distance));
			mesh->BindUniformsLowDetail(camera, modelMatrix, viewMatrix, projectionMatrix);
			mesh->RenderLowDetail();
		}
		else if (LOD->GetCurrentLODType(distance) == MODEL_LOD) {
			Model* model;
			model = ModelManager::GetInstance()->GetModel(LOD->GetCurrentLODName(distance));
			model->BindUniformsLowDetail(camera, modelMatrix, viewMatrix, projectionMatrix);
			model->RenderLowDetail();
		}
	}
}

void GameObject::RenderDepth(glm::mat4 modelMatrix, glm::mat4 lightSpaceMatrix) {
	if (meshName != "") {
		Mesh* mesh;
		mesh = MeshManager::GetInstance()->GetMesh(meshName);
		mesh->BindUniformsDepth(modelMatrix, lightSpaceMatrix);
		mesh->RenderDepth();
	}
	else if (modelName != "") {
		Model* model;
		model = ModelManager::GetInstance()->GetModel(modelName);
		model->BindUniformsDepth(modelMatrix, lightSpaceMatrix);
		model->RenderDepth();
	}
	else if (LOD != nullptr && highQualityShadows && LOD->GetFirstLODType() == MESH_LOD) {
		Mesh* mesh;
		mesh = MeshManager::GetInstance()->GetMesh(LOD->GetFirstLODName());
		mesh->BindUniformsDepth(modelMatrix, lightSpaceMatrix);
		mesh->RenderDepth();
	}
	else if (LOD != nullptr && highQualityShadows && LOD->GetFirstLODType() == MODEL_LOD) {
		Model* model;
		model = ModelManager::GetInstance()->GetModel(LOD->GetFirstLODName());
		model->BindUniformsDepth(modelMatrix, lightSpaceMatrix);
		model->RenderDepth();
	}
	else if (LOD != nullptr && !highQualityShadows && LOD->GetLastLODType() == MESH_LOD) {
		Mesh* mesh;
		mesh = MeshManager::GetInstance()->GetMesh(LOD->GetLastLODName());
		mesh->BindUniformsDepth(modelMatrix, lightSpaceMatrix);
		mesh->RenderDepth();
	}
	else if (LOD != nullptr && !highQualityShadows && LOD->GetLastLODType() == MODEL_LOD) {
		Model* model;
		model = ModelManager::GetInstance()->GetModel(LOD->GetLastLODName());
		model->BindUniformsDepth(modelMatrix, lightSpaceMatrix);
		model->RenderDepth();
	}
}

void GameObject::Render(Shader *shader) {
	if (meshName != "") {
		Mesh* mesh;
		mesh = MeshManager::GetInstance()->GetMesh(meshName);
		shader->Use();
		shader->SetMat4("model", modelMatrix);
		mesh->Render();
	}
	else if (modelName != "") {
		Model* model;
		model = ModelManager::GetInstance()->GetModel(modelName);
		shader->Use();
		shader->SetMat4("model", modelMatrix);
		model->Draw();
	}
}

void GameObject::PostRender() {
	if (meshName != "") {
		Mesh* mesh;
		mesh = MeshManager::GetInstance()->GetMesh(meshName);
		mesh->PostRender();
	}
	else if (modelName != "") {
		Model* model;
		model = ModelManager::GetInstance()->GetModel(modelName);
		model->PostRender();
	}
}

void GameObject::Update(float deltaTime) {

}