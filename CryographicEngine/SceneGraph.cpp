#include "SceneGraph.h"

SceneGraph::SceneGraph() {
	rootSceneNode = new SceneNode();
}

SceneGraph::~SceneGraph() {
	delete rootSceneNode;
	rootSceneNode = nullptr;
}

void SceneGraph::RenderSceneNode(SceneNode *sceneRoot, Frustum &frustum, Window *window, Camera *camera, CubeMap *skybox) {
	glm::mat4 viewMatrix = glm::mat4(camera->GetViewMatrix());
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera->GetFOV()), static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight()), camera->GetNear(), camera->GetFar());

	matStk.PushModelMatrix();
	matStk.Translate(sceneRoot->GetPosition());
	matStk.Rotate(sceneRoot->GetRotation());
	matStk.Scale(sceneRoot->GetScale());

	SceneNode::objectIterator it = sceneRoot->ObjectBegin();
	while (it != sceneRoot->ObjectEnd()) {
		Object *object = *it;
	
		skybox->BindTexture();
		object->PreRender();
		object->Render(camera, lightList, matStk.GetModelMatrix(), viewMatrix, projectionMatrix);
		object->PostRender();
		
		it++;
	}

	SceneNode *child = sceneRoot->GetFirstChild();
	while (child != nullptr) {
		RenderSceneNode(child, frustum,  window, camera, skybox);
		child = child->GetNextSibling();
	}

	matStk.PopModelMatrix();
}

void SceneGraph::RenderLowDetailSceneNode(SceneNode *sceneRoot, Frustum &frustum, Window *window, Camera *camera, glm::mat4 view, glm::mat4 projection, CubeMap* skybox) {
	glm::mat4 viewMatrix = glm::mat4(camera->GetViewMatrix());
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera->GetFOV()), static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight()), camera->GetNear(), camera->GetFar());

	matStk.PushModelMatrix();
	matStk.Translate(sceneRoot->GetPosition());
	matStk.Rotate(sceneRoot->GetRotation());
	matStk.Scale(sceneRoot->GetScale());

	SceneNode::objectIterator it = sceneRoot->ObjectBegin();
	while (it != sceneRoot->ObjectEnd()) {
		Object *object = *it;

		object->PreRender();
		object->RenderLowDetail(camera, matStk.GetModelMatrix(), view, projection);
		object->PostRender();

		it++;
	}

	SceneNode *child = sceneRoot->GetFirstChild();
	while (child != nullptr) {
		RenderLowDetailSceneNode(child, frustum, window, camera, view, projection, skybox);
		child = child->GetNextSibling();
	}

	matStk.PopModelMatrix();
}

void SceneGraph::RenderDepthSceneNode(SceneNode *sceneRoot, Frustum &frustum, Window *window, Camera* camera, Light *light, int index) {

	matStk.PushModelMatrix();
	matStk.Translate(sceneRoot->GetPosition());
	matStk.Rotate(sceneRoot->GetRotation());
	matStk.Scale(sceneRoot->GetScale());

	SceneNode::objectIterator it = sceneRoot->ObjectBegin();
	while (it != sceneRoot->ObjectEnd()) {
		Object *object = *it;

		if (object->CanCastShadows()) {
			object->RenderDepth(matStk.GetModelMatrix(), light->GetLightSpaceMatrix(camera, window, index));
		}

		it++;
	}

	SceneNode *child = sceneRoot->GetFirstChild();
	while (child != nullptr) {
		RenderDepthSceneNode(child, frustum, window, camera, light, index);
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



void SceneGraph::RenderSceneGraph(Frustum &frustum, Window *window, Camera *camera, CubeMap* skybox) {
	objectList = GetSceneObjects();
	lightList = GetSceneLights();
	RenderSceneNode(rootSceneNode, frustum, window, camera, skybox);
}

void SceneGraph::RenderLowDetailSceneGraph(Frustum &frustum, Window *window, Camera *camera, glm::mat4 view, glm::mat4 projection, CubeMap* skybox) {
	RenderLowDetailSceneNode(rootSceneNode, frustum, window, camera, view, projection, skybox);
}

void SceneGraph::RenderDepthSceneGraph(Frustum &frustum, Window *window, Camera *camera, Light *light, int index) {
	glCullFace(GL_FRONT);
	RenderDepthSceneNode(rootSceneNode, frustum, window, camera, light, index);
	glCullFace(GL_FRONT);
}

