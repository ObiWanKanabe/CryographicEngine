#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "AbstractRenderer.h"
#include "ShaderManager.h"
#include "MatrixStack.h"

class OpenGLRenderer : public AbstractRenderer{
private:
	// VAOs and VBOs of the final screen quad
	GLuint VAO, VBO;
	
	// The multisampled frame buffer object and colour output texture
	GLuint MSFBO, MStextureColourBuffer;

	// The non multisampled frame buffer object and colour output texture texture of the final screen quad
	GLuint FBO, textureColourBuffer;

	// Depth Map FBO and buffer texture for testing
	GLuint depthMapFBO, depthMapTexture;

	// List of vertices used when drawing to the final screen quad
	std::vector<GLfloat> vertices;

	// Shader to be used when drawing the final screen quad
	Shader* shader;
	Shader* exposureShader;

	// Storing a list to all the lights in the scene
	std::vector<Light*> lights;

	// MSAA Properties
	bool MSAA; // Wether MSAA is enabled or not
	unsigned int samples; // Number of samples used in MSAA

	// HDR Properties
	bool exposureMode; // Wether different exposure mode is enabled or not
	float exposure; // Exposure value
	float gamma; // Gamma value to adjust for different display's brightness

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

	// MSAA Functions
	virtual void SetMSAA(bool _MSAA, Window *window) override;
	virtual void ToggleMSAA(Window *window) override;
	virtual void SetMSAASamples(unsigned int _samples, Window *window) override;

	// HDR Functions
	virtual void SetExposureMode(bool _exposure) override;
	virtual void ToggleExposureMode() override;
	virtual void SetExposure(float _exposure) override;
	virtual void SetGamma(float _gamma) override;
	virtual float GetExposure() override;
	virtual float GetGamma() override;
};
#endif

