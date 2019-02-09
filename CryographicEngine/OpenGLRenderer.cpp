#include "OpenGLRenderer.h"
#include <iostream>

OpenGLRenderer::OpenGLRenderer(Window *window) {
	_closed = !Init(window);

}

OpenGLRenderer::~OpenGLRenderer() {

}

bool OpenGLRenderer::Init(Window *window) {

	shader = new Shader("../Shaders/screen.vs", "../Shaders/screenHDR.fs");
	exposureShader = new Shader("../Shaders/screen.vs", "../Shaders/screenHDRExposure.fs");
	blurShader = new Shader("../Shaders/gaussianBlur.vs", "../Shaders/gaussianBlur.fs");
	//shader = new Shader("../Shaders/shadowDebug.vs", "../Shaders/shadowDebug.fs");

	// Intialization of frame buffers here
	InitFrameBuffers(window);

	// The vertices of our quad, no transformations necessary since it's being place right at the screen
	vertices = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	// Generating the VAOs and VBOs for our final screen quad
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	return true;
}

bool OpenGLRenderer::InitFrameBuffers(Window *window) {

	if (Settings::GetInstance()->GetVideoSettingBool(MSAA) && Settings::GetInstance()->GetVideoSettingBool(BLOOM)) {
		// Initializing the multi-sampled framebuffer object and putting it into a texture
		// This will be blitted into another framebuffer and texture for post-processing
		// Here we're using two colourbuffers so we can seperate the normal colour output from
		// the bright colour output we will be blurring for the bloom effect
		glGenFramebuffers(1, &MSFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
		glGenTextures(2, MStextureColourBuffer);
		for (int i = 0; i < 2; i++)
		{
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, MStextureColourBuffer[i]);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Settings::GetInstance()->GetVideoSettingInt(MSAA_SAMPLES), GL_RGBA16F, window->GetWidth(), window->GetHeight(), GL_TRUE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, MStextureColourBuffer[i], 0);
		}

		// We're attaching this renderbuffer object here
		// this will take up the depth part of our framebuffer
		// No need to save it and draw it on the quad
		GLuint RBO;
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, Settings::GetInstance()->GetVideoSettingInt(MSAA_SAMPLES), GL_DEPTH_COMPONENT, window->GetWidth(), window->GetHeight());
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

		// We're letting OpenGL know we are drawing to two colour buffers
		GLuint colourAttachements[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, colourAttachements);

		// Check if the multi-sampled framebuffer has been completed, then continue
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "ERROR::FRAMEBUFFER:: Multi-Sampled Framebuffer is not complete!" << std::endl;
			return false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Initializing the non-multisampled framebuffer for post-processing and putting it into a texture
		// Only need a colour buffer for this one
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glGenTextures(2, textureColourBuffer);
		for (int i = 0; i < 2; i++)
		{
			glBindTexture(GL_TEXTURE_2D, textureColourBuffer[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window->GetWidth(), window->GetHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureColourBuffer[i], 0);
		}

		// We're letting OpenGL know we are drawing to two colour buffers
		glDrawBuffers(2, colourAttachements);

		// Check if the framebuffer has been completed, then continue
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
			return false;
		}

		// We're making the gaussian blur framebuffer objects and textures so that 
		// we can blur the extracted parts of the scene horizontally and vertically
		glGenFramebuffers(2, blurFBO);
		glGenTextures(2, blurColourBuffers);
		for (unsigned int i = 0; i < 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[i]);
			glBindTexture(GL_TEXTURE_2D, blurColourBuffers[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window->GetWidth(), window->GetHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurColourBuffers[i], 0);

			// Check if the framebuffer has been completed, then continue
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				std::cout << "ERROR::FRAMEBUFFER:: Blur Framebuffer is not complete!" << std::endl;
				return false;
			}
		}
	} else if (!Settings::GetInstance()->GetVideoSettingBool(MSAA) && Settings::GetInstance()->GetVideoSettingBool(BLOOM)) {
		// Initializing the final framebuffer object and putting it into a texture
		// We're only going to put this coloured texture on the final screen quad
		// Here we're using two colourbuffers so we can seperate the normal colour output from
		// the bright colour output we will be blurring for the bloom effect
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glGenTextures(2, textureColourBuffer);
		for (int i = 0; i < 2; i++) 
		{
			glBindTexture(GL_TEXTURE_2D, textureColourBuffer[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window->GetWidth(), window->GetHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureColourBuffer[i], 0);
		}

		// We're attaching this renderbuffer object here
		// this will take up the depth and stencil part of our framebuffer
		// No need to save it and draw it on the quad
		GLuint RBO;
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window->GetWidth(), window->GetHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

		// We're letting OpenGL know we are drawing to two colour buffers
		GLuint colourAttachements[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, colourAttachements);

		// Check if the framebuffer has been completed, then continue
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
			return false;
		}

		// We're making the gaussian blur framebuffer objects and textures so that 
		// we can blur the extracted parts of the scene horizontally and vertically
		glGenFramebuffers(2, blurFBO);
		glGenTextures(2, blurColourBuffers);
		for (unsigned int i = 0; i < 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[i]);
			glBindTexture(GL_TEXTURE_2D, blurColourBuffers[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window->GetWidth(), window->GetHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurColourBuffers[i], 0);
			
			// Check if the framebuffer has been completed, then continue
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				std::cout << "ERROR::FRAMEBUFFER:: Blur Framebuffer is not complete!" << std::endl;
				return false;
			}
		}

	} else if (Settings::GetInstance()->GetVideoSettingBool(MSAA)) {
	// Initializing the multi-sampled framebuffer object and putting it into a texture
	// This will be blitted into another framebuffer and texture for post-processing
	glGenFramebuffers(1, &MSFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
	glGenTextures(1, &MStextureColourBuffer[0]);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, MStextureColourBuffer[0]);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Settings::GetInstance()->GetVideoSettingInt(MSAA_SAMPLES), GL_RGBA16F, window->GetWidth(), window->GetHeight(), GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, MStextureColourBuffer[0], 0);

	// We're attaching this renderbuffer object here
	// this will take up the depth and stencil part of our framebuffer
	// No need to save it and draw it on the quad
	GLuint RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, Settings::GetInstance()->GetVideoSettingInt(MSAA_SAMPLES), GL_DEPTH_COMPONENT, window->GetWidth(), window->GetHeight());
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// Check if the multi-sampled framebuffer has been completed, then continue
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER:: Multi-Sampled Framebuffer is not complete!" << std::endl;
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Initializing the non-multisampled framebuffer for post-processing and putting it into a texture
	// Only need a colour buffer for this one
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glGenTextures(1, &textureColourBuffer[0]);
	glBindTexture(GL_TEXTURE_2D, textureColourBuffer[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window->GetWidth(), window->GetHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColourBuffer[0], 0);

	// Check if the framebuffer has been completed, then continue
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		return false;
	}

	} else if (!Settings::GetInstance()->GetVideoSettingBool(MSAA)) {
		// Initializing the final framebuffer object and putting it into a texture
		// We're only going to put this coloured texture on the final screen quad
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glGenTextures(1, &textureColourBuffer[0]);
		glBindTexture(GL_TEXTURE_2D, textureColourBuffer[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window->GetWidth(), window->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColourBuffer[0], 0);

		// We're attaching this renderbuffer object here
		// this will take up the depth and stencil part of our framebuffer
		// No need to save it and draw it on the quad
		GLuint RBO;
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window->GetWidth(), window->GetHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

		// Check if the framebuffer has been completed, then continue
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
			return false;
		}
	} 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void OpenGLRenderer::SetUpTextureBuffer(GLuint _FBO, GLuint _texture, unsigned int _totalTextures, unsigned int _index, bool _multisampled, bool _firstTexture, Window *window) {
	if (_firstTexture) {
		glGenTextures(_totalTextures, &_texture);
	}
	glBindTexture(GL_TEXTURE_2D, _texture);
	if (!_multisampled) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window->GetWidth(), window->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);
	} else if (_multisampled) {
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Settings::GetInstance()->GetVideoSettingInt(MSAA_SAMPLES), GL_RGBA16F, window->GetWidth(), window->GetHeight(), GL_TRUE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _index, GL_TEXTURE_2D_MULTISAMPLE, _texture, 0);
	}

}

void OpenGLRenderer::SetUpRenderBuffer(bool _multisampled, Window *window) {
	if (_multisampled) {
		GLuint RBO;
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, Settings::GetInstance()->GetVideoSettingInt(MSAA_SAMPLES), GL_DEPTH_COMPONENT, window->GetWidth(), window->GetHeight());
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);
	}
	else {
		GLuint RBO;
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window->GetWidth(), window->GetHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);
	}
}

void OpenGLRenderer::PreRender(Window *window, Camera *camera, CubeMap *skybox) {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::Render(Window *window, Frustum &frustum, Camera *camera, CubeMap *skybox, SceneGraph *scenegraph) {
	glm::mat4 viewMatrix = glm::mat4(camera->GetViewMatrix());
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera->GetFOV()), static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight()), 0.1f, 100.0f);

	lights = scenegraph->GetSceneLights();

	// Rendering Shadow Maps to the one directional light in the scene

	for (unsigned int i = 0; i < lights.size(); i++) {
		if (lights[i]->HasShadows()) {
			int renders = lights[i]->GetNumDepthMaps();
			for (int j = 0; j < renders; j++) {
				lights[i]->PrepareShadow(j);
				scenegraph->RenderDepthSceneGraph(frustum, window, camera, lights[i], j);
			}
		}
	}

	// Changing viewport back to window values
	glViewport(0, 0, window->GetWidth(), window->GetHeight());

	// Switching to our final screen framebuffer and clearing the buffers
	if (Settings::GetInstance()->GetVideoSettingBool(MSAA))
		glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
	else if (!Settings::GetInstance()->GetVideoSettingBool(MSAA))
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glEnable(GL_DEPTH_TEST);
	window->Clear();

	skybox->PreRender();

	scenegraph->RenderSceneGraph(frustum, window, camera, skybox);

	skybox->BindUniforms(viewMatrix, projectionMatrix);
	skybox->Render();

}

void OpenGLRenderer::PostRender(Window *window, Camera *camera, CubeMap *skybox) {
	skybox->PostRender();

	// Using the multisampled buffer and blitting the image to the non-multisampled buffer
	// Doing this so we can apply post processing effects while using MSAA
	// For bloom we have to blit it twice for two different texture components
	if (Settings::GetInstance()->GetVideoSettingBool(MSAA) && Settings::GetInstance()->GetVideoSettingBool(BLOOM)) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, MSFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
		for (unsigned int i = 0; i < 2; i++) {
			glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
			glBlitFramebuffer(0, 0, window->GetWidth(), window->GetHeight(), 0, 0, window->GetWidth(), window->GetHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
		}
	}
	// Using the multisampled buffer and blitting the image to the non-multisampled buffer
	// Doing this so we can apply post processing effects while using MSAA
	else if (Settings::GetInstance()->GetVideoSettingBool(MSAA)) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, MSFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
		glBlitFramebuffer(0, 0, window->GetWidth(), window->GetHeight(), 0, 0, window->GetWidth(), window->GetHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	// If bloom is enabled, we're putting the texture through a gaussian blur algorithm
	// It will go through the different FBOs and their texture buffers until it finished the desired passes
	if (Settings::GetInstance()->GetVideoSettingBool(BLOOM)) {
		bool horizontal = true;
		bool first_iteration = true;
		unsigned int passes = Settings::GetInstance()->GetVideoSettingInt(BLOOM_PASSES);
		blurShader->Use();
		for (unsigned int i = 0; i < passes; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[horizontal]);
			blurShader->SetInt("horizontal", horizontal);
			glBindTexture(GL_TEXTURE_2D, first_iteration ? textureColourBuffer[1] : blurColourBuffers[!horizontal]);
			RenderQuad();
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// Switching back to the default framebuffer, and disabling depth testing
	// so our quad is rendered normally on the screen
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, window->GetWidth(), window->GetHeight());

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Drawing the final screen quad
	// This Exposure mode option allows the developer to change the exposure of the scene
	// based on the brightness of lights or due to other reasons
	if (Settings::GetInstance()->GetVideoSettingBool(EXPOSURE) && Settings::GetInstance()->GetVideoSettingBool(BLOOM)) {
		exposureShader->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColourBuffer[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, blurColourBuffers[!horizontal]);
		exposureShader->SetFloat("exposure", Settings::GetInstance()->GetVideoSettingFloat(EXPOSURE_VALUE));
		exposureShader->SetFloat("gamma", Settings::GetInstance()->GetVideoSettingFloat(GAMMA));
		exposureShader->SetInt("screenTexture", 0);
		exposureShader->SetInt("bloomBlur", 1);
	}
	else if (Settings::GetInstance()->GetVideoSettingBool(EXPOSURE) && !Settings::GetInstance()->GetVideoSettingBool(BLOOM)) {
		exposureShader->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColourBuffer[0]);
		exposureShader->SetFloat("exposure", Settings::GetInstance()->GetVideoSettingFloat(EXPOSURE_VALUE));
		exposureShader->SetFloat("gamma", Settings::GetInstance()->GetVideoSettingFloat(GAMMA));
		exposureShader->SetInt("screenTexture", 0);
	}
	// Disabling exposure mode simplifies the developers lighting choices and still uses
	// a simple HDR algorithm that can read lighting values outisde the 0 - 1 range
	else if (!Settings::GetInstance()->GetVideoSettingBool(EXPOSURE) && Settings::GetInstance()->GetVideoSettingBool(BLOOM)) {
		shader->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColourBuffer[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, blurColourBuffers[!horizontal]);
		shader->SetFloat("gamma", Settings::GetInstance()->GetVideoSettingFloat(GAMMA));
		shader->SetInt("screenTexture", 0);
		shader->SetInt("bloomBlur", 1);
	}
	else {
		shader->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColourBuffer[0]);
		shader->SetFloat("gamma", Settings::GetInstance()->GetVideoSettingFloat(GAMMA));
	}
	
	RenderQuad();
	/*for (int i = 0; i < lights.size(); i++) {
		if (lights[i]->GetType() == LIGHT_TYPE::DIRECTIONAL_LIGHT)
			glBindTexture(GL_TEXTURE_2D, lights[i]->GetShadowMap());
	}*/

	window->DoubleBuffer();
}

void OpenGLRenderer::RenderQuad() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void OpenGLRenderer::Clear() {

}

void OpenGLRenderer::ToggleMSAA(Window *window) {
	if (Settings::GetInstance()->GetVideoSettingBool(MSAA)) {
		SetMSAA(false, window);
	}
	else if (!Settings::GetInstance()->GetVideoSettingBool(MSAA)) {
		SetMSAA(true, window);
	}
}

void OpenGLRenderer::SetMSAA(bool _MSAA, Window *window) {
	Settings::GetInstance()->SetVideoSettingBool(MSAA, _MSAA);
	InitFrameBuffers(window);
}

void OpenGLRenderer::SetMSAASamples(unsigned int _samples, Window *window) {
	int samples = glm::clamp(static_cast<int>(_samples), 0, 16);
	Settings::GetInstance()->SetVideoSettingInt(MSAA_SAMPLES, _samples);
	InitFrameBuffers(window);
}

bool OpenGLRenderer::GetMSAAState() {
	return Settings::GetInstance()->GetVideoSettingBool(MSAA);
}

int OpenGLRenderer::GetMSAASamples() {
	return Settings::GetInstance()->GetVideoSettingInt(MSAA_SAMPLES);
}

void OpenGLRenderer::ToggleExposureMode() {
	if (Settings::GetInstance()->GetVideoSettingBool(EXPOSURE)) {
		SetExposureMode(false);
	}
	else if (!Settings::GetInstance()->GetVideoSettingBool(EXPOSURE)) {
		SetExposureMode(true);
	}
}

void OpenGLRenderer::SetExposureMode(bool _exposure) {
	Settings::GetInstance()->SetVideoSettingBool(EXPOSURE, _exposure);
}

void OpenGLRenderer::SetExposure(float _exposure) {
	float exposure = glm::clamp(_exposure, 0.0f, 10.0f);
	Settings::GetInstance()->SetVideoSettingFloat(EXPOSURE_VALUE, exposure);
}

void OpenGLRenderer::SetGamma(float _gamma) {
	float gamma = glm::clamp(_gamma, 0.0f, 10.0f);
	Settings::GetInstance()->SetVideoSettingFloat(GAMMA, gamma);
}

bool OpenGLRenderer::GetExposureState() {
	return Settings::GetInstance()->GetVideoSettingBool(EXPOSURE);
}
float OpenGLRenderer::GetExposure() {
	return Settings::GetInstance()->GetVideoSettingFloat(EXPOSURE_VALUE);
}

float OpenGLRenderer::GetGamma() {
	return Settings::GetInstance()->GetVideoSettingFloat(GAMMA);
}

void OpenGLRenderer::ToggleBloom(Window *window) {
	if (Settings::GetInstance()->GetVideoSettingBool(BLOOM)) {
		SetBloom(false, window);
	}
	else if (!Settings::GetInstance()->GetVideoSettingBool(BLOOM)) {
		SetBloom(true, window);
	}
}

void OpenGLRenderer::SetBloom(bool _bloom, Window *window) {
	Settings::GetInstance()->SetVideoSettingBool(BLOOM, _bloom);
	InitFrameBuffers(window);
}

void OpenGLRenderer::SetBloomPasses(int _passes) {
	Settings::GetInstance()->SetVideoSettingInt(BLOOM_PASSES, _passes);
}

bool OpenGLRenderer::GetBloomState() {
	return Settings::GetInstance()->GetVideoSettingBool(BLOOM);
}

int OpenGLRenderer::GetBloomPasses() {
	return Settings::GetInstance()->GetVideoSettingInt(BLOOM_PASSES);
}