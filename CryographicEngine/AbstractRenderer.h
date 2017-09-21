#ifndef ABSTRACTRENDERER_H
#define ABSTRACTRENDERER_H

#include "Window.h"
#include "GameEngine.h"

class AbstractRenderer {
public:
	enum primitiveType {};
	virtual void renderPrimitive(primitiveType prim) = 0;
};
#endif

