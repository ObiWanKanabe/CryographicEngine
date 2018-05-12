#include "OpenGLRenderer.h"
#include <iostream>

OpenGLRenderer::OpenGLRenderer(){
	_closed = !Init();

}

OpenGLRenderer::~OpenGLRenderer() {
	
}

bool OpenGLRenderer::Init() {
	return true;
}

void OpenGLRenderer::PreRender(Window *window, Camera *camera, CubeMap *skybox) {
	skybox->PreRender();
}

void OpenGLRenderer::Render(Window *window, Camera *camera, CubeMap *skybox) {
	glm::mat4 viewMatrix = glm::mat4(camera->GetViewMatrix());
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera->GetFOV()), static_cast<float>(window->GetWidth() / window->GetHeight()), 0.1f, 100.0f);

	skybox->BindUniforms(viewMatrix, projectionMatrix);
	skybox->Render();

}

void OpenGLRenderer::PostRender(Window *window, Camera *camera, CubeMap *skybox) {
	skybox->PostRender();
	window->DoubleBuffer();
}

void OpenGLRenderer::Clear() {
	
}