#ifndef ABSTRACTRENDERER_H
#define ABSTRACTRENDERER_H

#include "Window.h"
#include "Camera.h"
#include "MeshManager.h"
#include "CubeMap.h"
#include "SceneNode.h"

class AbstractRenderer {
public:
	explicit AbstractRenderer() {};
	virtual ~AbstractRenderer() {};

	virtual bool Init(Window *window) = 0;

	virtual void PreRender(Window *window, Camera *camera, CubeMap *skybox) = 0;
	virtual void Render(Window *window, Camera *camera, CubeMap *skybox) = 0;
	virtual void PostRender(Window *window, Camera *camera, CubeMap *skybox) = 0;

	virtual void Clear() = 0;

protected:
	bool _closed = false;
};
#endif

