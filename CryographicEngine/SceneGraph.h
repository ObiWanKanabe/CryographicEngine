#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "SceneNode.h"
#include "AbstractRenderer.h"
#include "Camera.h"
#include "CubeMap.h"
#include "Frustum.h"
#include "MatrixStack.h"

class SceneGraph {
private:
	SceneNode *rootSceneNode;
	MatrixStack matStk;

	void RenderSceneNode(SceneNode *sceneRoot, Frustum &frustum, AbstractRenderer &renderer, Camera *camera, CubeMap* skybox);

public:

	SceneGraph();
	~SceneGraph();

	SceneNode* GetRootSceneNode();
	void RenderSceneGraph(Frustum &frustum, AbstractRenderer &renderer, Camera *camera, CubeMap* skybox);



};
#endif

