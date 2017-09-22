#ifndef ABSTRACTRENDERER_H
#define ABSTRACTRENDERER_H

#include "Window.h"

class AbstractRenderer {
public:
	explicit AbstractRenderer() {};
	virtual ~AbstractRenderer() {};

	virtual bool init(Window *window) = 0;
	virtual void renderPrimitive() = 0;

protected:
	bool _closed = false;
};
#endif

