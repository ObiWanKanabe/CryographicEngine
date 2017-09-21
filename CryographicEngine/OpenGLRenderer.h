#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "AbstractRenderer.h"

class OpenGLRenderer {
public:
	virtual void renderPrimitive(AbstractRenderer::primitiveType prim);

};
#endif

