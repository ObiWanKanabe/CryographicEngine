#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "AbstractRenderer.h"
#include "Shader.h"
#include "Mesh.h"

class OpenGLRenderer : public AbstractRenderer{
public:
	explicit OpenGLRenderer();
	virtual ~OpenGLRenderer();
	virtual bool init() override;
	virtual void renderPrimitive(Window *window) override;
	virtual void clear() override;
private:
	GLuint VAO, VBO;
	Shader *shader;
	Mesh *mesh;
};
#endif

