#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "AbstractRenderer.h"
#include "ShaderManager.h"
#include "Triangle.h"

class OpenGLRenderer : public AbstractRenderer{
public:

	// Default constructor
	explicit OpenGLRenderer();

	// Default deconstructor
	virtual ~OpenGLRenderer();

	// Initialize all objects here
	virtual bool Init() override;

	// Renders meshes here
	virtual void RenderPrimitive(Window *window) override;

	// Clear renderer here
	virtual void Clear() override;
private:
	ResourceHandle<Shader> orangeish;
public:
};
#endif

