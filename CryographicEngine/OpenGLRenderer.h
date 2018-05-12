#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "AbstractRenderer.h"
#include "ShaderManager.h"
#include "MatrixStack.h"

class OpenGLRenderer : public AbstractRenderer{
private:

public:
	// Default constructor
	explicit OpenGLRenderer();

	// Default deconstructor
	virtual ~OpenGLRenderer();

	// Initialize all objects here
	virtual bool Init() override;

	// Pre render here
	virtual void PreRender(Window *window, Camera *camera, CubeMap *skybox) override;

	// Renders meshes here
	virtual void Render(Window *window, Camera *camera, CubeMap *skybox) override;

	// Post render here
	virtual void PostRender(Window *window, Camera *camera, CubeMap *skybox) override;

	// Clear renderer here
	virtual void Clear() override;
};
#endif

