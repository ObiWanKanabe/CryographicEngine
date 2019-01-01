#ifndef ABSTRACTRENDERER_H
#define ABSTRACTRENDERER_H

#include "Window.h"
#include "Camera.h"
#include "MeshManager.h"
#include "CubeMap.h"
#include "SceneGraph.h"

class AbstractRenderer {
public:
	explicit AbstractRenderer() {};
	virtual ~AbstractRenderer() {};

	virtual bool Init(Window *window) = 0;
	virtual bool InitFrameBuffers(Window *window) = 0;

	virtual void PreRender(Window *window, Camera *camera, CubeMap *skybox) = 0;
	virtual void Render(Window *window, Frustum &frustum, Camera *camera, CubeMap *skybox, SceneGraph *scenegraph) = 0;
	virtual void PostRender(Window *window, Camera *camera, CubeMap *skybox) = 0;

	virtual void Clear() = 0;

	virtual void SetMSAA(bool _MSAA, Window *window) = 0;
	virtual void ToggleMSAA(Window *window) = 0;
	virtual void SetMSAASamples(unsigned int _samples, Window *window) = 0;

protected:
	bool _closed = false;
};
#endif

