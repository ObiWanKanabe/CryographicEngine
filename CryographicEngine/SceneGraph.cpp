#include "SceneGraph.h"

SceneGraph::SceneGraph() {
	rootSceneNode = new SceneNode();
}

SceneGraph::~SceneGraph() {
	delete rootSceneNode;
	rootSceneNode = nullptr;
}

void SceneGraph::RenderSceneNode(SceneNode *sceneRoot, Frustum &frustum, Camera *camera, CubeMap *skybox) {
	glm::mat4 viewMatrix = glm::mat4(camera->GetViewMatrix());
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera->GetFOV()), 1200.0f / 900.0f, 0.1f, 100.0f);

	matStk.PushModelMatrix();
	matStk.Translate(sceneRoot->GetPosition());
	matStk.Rotate(sceneRoot->GetRotation());
	matStk.Scale(sceneRoot->GetScale());

	SceneNode::objectIterator it = sceneRoot->ObjectBegin();
	while (it != sceneRoot->ObjectEnd()) {
		Object *mesh = *it;
	
		skybox->BindTexture();
		mesh->PreRender();
		mesh->Render(camera, lightList, matStk.GetModelMatrix(), viewMatrix, projectionMatrix);
		mesh->PostRender();
		
		it++;
	}

	SceneNode *child = sceneRoot->GetFirstChild();
	while (child != nullptr) {
		RenderSceneNode(child, frustum,  camera, skybox);
		child = child->GetNextSibling();
	}

	matStk.PopModelMatrix();
}

void SceneGraph::RenderLowDetailSceneNode(SceneNode *sceneRoot, Frustum &frustum, Camera *camera, glm::mat4 view, glm::mat4 projection, CubeMap* skybox) {
	glm::mat4 viewMatrix = glm::mat4(camera->GetViewMatrix());
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera->GetFOV()), 1200.0f / 900.0f, 0.1f, 100.0f);

	matStk.PushModelMatrix();
	matStk.Translate(sceneRoot->GetPosition());
	matStk.Rotate(sceneRoot->GetRotation());
	matStk.Scale(sceneRoot->GetScale());

	SceneNode::objectIterator it = sceneRoot->ObjectBegin();
	while (it != sceneRoot->ObjectEnd()) {
		Object *mesh = *it;

		skybox->BindTexture();
		mesh->PreRender();
		mesh->RenderLowDetail(camera, matStk.GetModelMatrix(), view, projection);
		mesh->PostRender();

		it++;
	}

	SceneNode *child = sceneRoot->GetFirstChild();
	while (child != nullptr) {
		RenderLowDetailSceneNode(child, frustum, camera, view, projection, skybox);
		child = child->GetNextSibling();
	}

	matStk.PopModelMatrix();
}

SceneNode* SceneGraph::GetRootSceneNode() {
	return rootSceneNode;
}

std::vector<Light*> SceneGraph::GetLights(SceneNode *sceneRoot) {

	std::vector<Light*> lightList;
	SceneNode::objectIterator it = sceneRoot->ObjectBegin();

	while (it != sceneRoot->ObjectEnd()) {
		Object *lightObject = *it;
		if (lightObject->GetAttachedLight() != nullptr) {
			lightList.push_back(lightObject->GetAttachedLight());
		}
		it++;
	}

	SceneNode *child = sceneRoot->GetFirstChild();
	while (child != nullptr) {
		std::vector<Light*> childLights = GetLights(child);
		lightList.insert(std::end(lightList), std::begin(childLights), std::end(childLights));
		child = child->GetNextSibling();
	}

	return lightList;
}

std::vector<Light*> SceneGraph::GetSceneLights() {
	return GetLights(rootSceneNode);
}

std::vector<Object*> SceneGraph::GetObjects(SceneNode *sceneRoot) {
	matStk.PushModelMatrix();
	matStk.Translate(sceneRoot->GetPosition());
	matStk.Rotate(sceneRoot->GetRotation());
	matStk.Scale(sceneRoot->GetScale());

	std::vector<Object*> objectList;
	SceneNode::objectIterator it = sceneRoot->ObjectBegin();

	while (it != sceneRoot->ObjectEnd()) {
		Object *object = *it;
		object->SetModelMatrix(matStk.GetModelMatrix());
		objectList.push_back(object);
		it++;
	}

	SceneNode *child = sceneRoot->GetFirstChild();
	while (child != nullptr) {
		std::vector<Object*> childObjects = GetObjects(child);
		objectList.insert(std::end(objectList), std::begin(childObjects), std::end(childObjects));
		child = child->GetNextSibling();
	}

	matStk.PopModelMatrix();
	return objectList;
}

std::vector<Object*> SceneGraph::GetSceneObjects() {
	return GetObjects(rootSceneNode);
}



void SceneGraph::RenderSceneGraph(Frustum &frustum, Camera *camera, CubeMap* skybox) {
	objectList = GetSceneObjects();
	lightList = GetSceneLights();
	RenderSceneNode(rootSceneNode, frustum, camera, skybox);
}

void SceneGraph::RenderLowDetailSceneGraph(Frustum &frustum, Camera *camera, glm::mat4 view, glm::mat4 projection, CubeMap* skybox) {
	RenderLowDetailSceneNode(rootSceneNode, frustum, camera, view, projection, skybox);
}

void SceneGraph::Render(Frustum &frustum, Camera *camera, CubeMap* skybox) {
	objectList = GetSceneObjects();
	lightList = GetSceneLights();

	glm::mat4 viewMatrix = glm::mat4(camera->GetViewMatrix());
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera->GetFOV()), SCREEN_WDITH / SCREEN_HEIGHT, 0.1f, 100.0f);

	// We're going through the lights here first, to calculate the shadow depth map
	// This happens to every light so each one has their own calculations for each object
	/*for (size_t i = 0; i < lightList.size(); i++) {
	lightList[i]->BindSpaceMatrix();
	for (size_t j = 0; j < objectList.size(); j++) {
	objectList[j]->Render(lightList[i]->GetShader());
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	glViewport(0, 0, 1200, 900);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/

	// Now that the shadows for each light and their depth map have been calculated
	// Continue like normal and pass the lights and their information to each object
	for (size_t i = 0; i < objectList.size(); i++) {
		skybox->BindTexture();
		objectList[i]->PreRender();
		objectList[i]->Render(camera, lightList, objectList[i]->GetModelMatrix(), viewMatrix, projectionMatrix);
		objectList[i]->PostRender();
	}
}

