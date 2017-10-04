#ifndef ABSTRACTRENDERER_H
#define ABSTRACTRENDERER_H

#include "Window.h"
#include "ResourceManager.h"

class AbstractRenderer {
public:
	explicit AbstractRenderer() {};
	virtual ~AbstractRenderer() {};

	virtual bool init() = 0;
	virtual void renderPrimitive(Window *window) = 0;
	virtual void clear() = 0;

protected:
	bool _closed = false;
};
#endif

