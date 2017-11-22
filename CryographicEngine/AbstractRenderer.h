#ifndef ABSTRACTRENDERER_H
#define ABSTRACTRENDERER_H

#include "Window.h"
#include "ResourceManager.h"
#include "ShaderManager.h"

class AbstractRenderer {
public:
	explicit AbstractRenderer() {};
	virtual ~AbstractRenderer() {};

	virtual bool Init() = 0;
	virtual void RenderPrimitive(Window *window) = 0;
	virtual void Clear() = 0;

protected:
	bool _closed = false;
};
#endif

