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
	GLuint MSFBO, MStextureColourBuffer[2];

	// The pingpong framebuffer object and colour output texture used in bloom effect
	// This is used to blur the bright parts stored from the previous framebuffer
	GLuint blurFBO[2], blurColourBuffers[2];

	bool horizontal;

	// The non multisampled frame buffer object and colour output texture texture of the final screen quad
	GLuint FBO, textureColourBuffer[2];

	// Depth Map FBO and buffer texture for testing
	GLuint depthMapFBO, depthMapTexture;

	// List of vertices used when drawing to the final screen quad
	std::vector<GLfloat> vertices;

	// Shader to be used when drawing the final screen quad
	Shader* shader;
	Shader* exposureShader;

	// shader to be used when blurring the bloom output
	Shader* blurShader;

	// Storing a list to all the lights in the scene
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

	// Sets up a texture buffer with default parameters
	virtual void SetUpTextureBuffer(GLuint _FBO, GLuint _texture, unsigned int _totalTextures, unsigned int _index, bool _multisampled,  bool _firstTexture, Window *window) override;

	// Sets up a render buffer object with default parameters
	virtual void SetUpRenderBuffer(bool _multisampled, Window *window) override;

	// Pre render here
	virtual void PreRender(Window *window, Camera *camera, CubeMap *skybox) override;

	// Renders meshes here
	virtual void Render(Window *window, Frustum &frustum, Camera *camera, CubeMap *skybox, SceneGraph *scenegraph) override;

	// Post render here
	virtual void PostRender(Window *window, Camera *camera, CubeMap *skybox) override;

	// Renders a quad to be drawn on
	virtual void RenderQuad() override;

	// Clear renderer here
	virtual void Clear() override;

	// MSAA Functions
	virtual void ToggleMSAA(Window *window) override;
	virtual void SetMSAA(bool _MSAA, Window *window) override;
	virtual void SetMSAASamples(unsigned int _samples, Window *window) override;
	virtual bool GetMSAAState() override;
	virtual int GetMSAASamples() override;

	// HDR Functions
	virtual void ToggleExposureMode() override;
	virtual void SetExposureMode(bool _exposure) override;
	virtual void SetExposure(float _exposure) override;
	virtual void SetGamma(float _gamma) override;
	virtual bool GetExposureState() override;
	virtual float GetExposure() override;
	virtual float GetGamma() override;

	// Bloom Functions
	virtual void ToggleBloom(Window *window) override;
	virtual void SetBloom(bool _bloom, Window *window) override;
	virtual void SetBloomPasses(float _passes) override;
	virtual bool GetBloomState() override;
	virtual int GetBloomPasses() override;
};
#endif

