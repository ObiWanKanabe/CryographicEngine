#include "SceneGraph.h"

SceneGraph::SceneGraph() {
	rootSceneNode = new SceneNode();
}

SceneGraph::~SceneGraph() {
	delete rootSceneNode;
}

void SceneGraph::RenderSceneNode(SceneNode *sceneRoot, Frustum &frustum, AbstractRenderer &renderer, Camera *camera, CubeMap *skybox) {
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
		RenderSceneNode(child, frustum, renderer, camera, skybox);
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

void SceneGraph::RenderSceneGraph(Frustum &frustum, AbstractRenderer &renderer, Camera *camera, CubeMap* skybox) {
	lightList = GetSceneLights();
	RenderSceneNode(rootSceneNode, frustum, renderer, camera, skybox);
}

