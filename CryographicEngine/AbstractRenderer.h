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
	virtual bool GetMSAAState() = 0;
	virtual int GetMSAASamples() = 0;

	virtual void ToggleExposureMode() = 0;
	virtual void SetExposureMode(bool _exposure) = 0;
	virtual void SetExposure(float _exposure) = 0;
	virtual void SetGamma(float _gamma) = 0;
	virtual bool GetExposureState() = 0;
	virtual float GetExposure() = 0;
	virtual float GetGamma() = 0;

protected:
	bool _closed = false;
};
#endif

