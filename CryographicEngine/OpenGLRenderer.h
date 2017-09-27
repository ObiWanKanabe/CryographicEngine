#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "AbstractRenderer.h"
#include "Shader.h"

class OpenGLRenderer : public AbstractRenderer{
public:
	explicit OpenGLRenderer(Window *window);
	virtual ~OpenGLRenderer();
	virtual bool init(Window *window) override;
	virtual void renderPrimitive();
private:
	Shader *shader;
};
#endif

