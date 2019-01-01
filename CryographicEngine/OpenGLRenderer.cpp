#include "OpenGLRenderer.h"
#include <iostream>

OpenGLRenderer::OpenGLRenderer(Window *window) {
	_closed = !Init(window);

}

OpenGLRenderer::~OpenGLRenderer() {

}

bool OpenGLRenderer::Init(Window *window) {

	shader = new Shader("../Shaders/screen.vs", "../Shaders/screen.fs");
	//shader = new Shader("../Shaders/shadowDebug.vs", "../Shaders/shadowDebug.fs");
	
	// Default MSAA attributes
	MSAA = true;
	samples = 4;

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

	if (MSAA) {
		// Initializing the final framebuffer object and putting it into a texture
		// We're only going to put this coloured texture on the final screen quad
		glGenFramebuffers(1, &MSFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
		glGenTextures(1, &MStextureColourBuffer);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, MStextureColourBuffer);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, window->GetWidth(), window->GetHeight(), GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, MStextureColourBuffer, 0);

		// We're attaching this renderbuffer object here
		// this will take up the depth and stencil part of our framebuffer
		// No need to save it and draw it on the quad
		GLuint RBO;
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, window->GetWidth(), window->GetHeight());
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

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
		glGenTextures(1, &textureColourBuffer);
		glBindTexture(GL_TEXTURE_2D, textureColourBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window->GetWidth(), window->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColourBuffer, 0);

		// Check if the framebuffer has been completed, then continue
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
			return false;
		}

	}
	else if (!MSAA) {
		// Initializing the final framebuffer object and putting it into a texture
		// We're only going to put this coloured texture on the final screen quad
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glGenTextures(1, &textureColourBuffer);
		glBindTexture(GL_TEXTURE_2D, textureColourBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window->GetWidth(), window->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColourBuffer, 0);

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

void OpenGLRenderer::PreRender(Window *window, Camera *camera, CubeMap *skybox) {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::Render(Window *window, Frustum &frustum, Camera *camera, CubeMap *skybox, SceneGraph *scenegraph) {
	glm::mat4 viewMatrix = glm::mat4(camera->GetViewMatrix());
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera->GetFOV()), static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight()), 0.1f, 100.0f);

	lights = scenegraph->GetSceneLights();

	// Rendering Shadow Maps to the one directional light in the scene

	for (int i = 0; i < lights.size(); i++) {
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
	if (MSAA)
		glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
	else if (!MSAA)
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
	if (MSAA) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, MSFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
		glBlitFramebuffer(0, 0, window->GetWidth(), window->GetHeight(), 0, 0, window->GetWidth(), window->GetHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	// Switching back to the default framebuffer, and disabling depth testing
	// so our quad is rendered normally on the screen
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, window->GetWidth(), window->GetHeight());

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Drawing the final screen quad
	shader->Use();
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureColourBuffer);
	/*for (int i = 0; i < lights.size(); i++) {
		if (lights[i]->GetType() == LIGHT_TYPE::DIRECTIONAL_LIGHT)
			glBindTexture(GL_TEXTURE_2D, lights[i]->GetShadowMap());
	}*/
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	window->DoubleBuffer();
}

void OpenGLRenderer::Clear() {

}

void OpenGLRenderer::SetMSAA(bool _MSAA, Window *window) {
	MSAA = _MSAA;
	InitFrameBuffers(window);
}

void OpenGLRenderer::ToggleMSAA(Window *window) {
	if (MSAA) {
		SetMSAA(false, window);
	}
	else if (!MSAA) {
		SetMSAA(true, window);
	}
}

void OpenGLRenderer::SetMSAASamples(unsigned int _samples, Window *window) {
	samples = _samples;
	InitFrameBuffers(window);
}