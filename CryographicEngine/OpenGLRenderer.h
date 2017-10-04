#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "AbstractRenderer.h"
#include "Shader.h"
#include "Mesh.h"

class OpenGLRenderer : public AbstractRenderer{
public:

	// Default constructor
	explicit OpenGLRenderer();

	// Default deconstructor
	virtual ~OpenGLRenderer();

	// Initialize all objects here
	virtual bool init() override;

	// Renders meshes here
	virtual void renderPrimitive(Window *window) override;

	// Clear renderer here
	virtual void clear() override;
private:

	GLuint VAO, VBO;
	Shader *shader;
	Mesh *mesh;
	ResourceManager<Mesh> *meshManager;
	ResourceHandle<Mesh> triangle;
};
#endif

