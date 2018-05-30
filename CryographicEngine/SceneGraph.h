#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "SceneNode.h"
#include "Camera.h"
#include "CubeMap.h"
#include "Frustum.h"
#include "MatrixStack.h"

// Taught in Game Engine Design Class @ Humber College

class SceneGraph {
private:
	SceneNode *rootSceneNode;
	MatrixStack matStk;
	std::vector<Light*> lightList;
	std::vector<Object*> objectList;

	void RenderSceneNode(SceneNode *sceneRoot, Frustum &frustum, Camera *camera, CubeMap* skybox);
	std::vector<Light*> GetLights(SceneNode *sceneRoot);
	std::vector<Object*> GetObjects(SceneNode *sceneRoot);

public:

	SceneGraph();
	~SceneGraph();

	SceneNode* GetRootSceneNode();
	std::vector<Light*> GetSceneLights();
	std::vector<Object*> GetSceneObjects();
	void RenderSceneGraph(Frustum &frustum, Camera *camera, CubeMap* skybox);
	void Render(Frustum &frustum, Camera *camera, CubeMap* skybox);

};
#endif

