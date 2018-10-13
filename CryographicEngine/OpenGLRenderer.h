#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "AbstractRenderer.h"
#include "ShaderManager.h"
#include "MatrixStack.h"

class OpenGLRenderer : public AbstractRenderer{
private:
	// VAOs and VBOs of the final screen quad
	GLuint VAO, VBO;
	
	// The frame buffer object and colour output texture of the final screen quad
	GLuint FBO, textureColourBuffer;

	// Depth Map FBO and buffer texture for testing
	GLuint depthMapFBO, depthMapTexture;

	// List of vertices used when drawing to the final screen quad
	std::vector<GLfloat> vertices;

	// Shader to be used when drawing the final screen quad
	Shader* shader;

	Light* shadowLight;

	std::vector<Light*> lights;

public:

	OpenGLRenderer() = delete;

	// Default constructor
	explicit OpenGLRenderer(Window *window);

	// Default deconstructor
	virtual ~OpenGLRenderer();

	// Initialize all buffer objects, vertices and texture here
	virtual bool Init(Window *window) override;

	// Initialize the buffer objects here
	virtual bool InitFrameBuffers(Window *window) override;

	// Pre render here
	virtual void PreRender(Window *window, Camera *camera, CubeMap *skybox) override;

	// Renders meshes here
	virtual void Render(Window *window, Frustum &frustum, Camera *camera, CubeMap *skybox, SceneGraph *scenegraph) override;

	// Post render here
	virtual void PostRender(Window *window, Camera *camera, CubeMap *skybox) override;

	// Clear renderer here
	virtual void Clear() override;
};
#endif

