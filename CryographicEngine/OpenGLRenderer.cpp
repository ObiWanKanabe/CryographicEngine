#include "OpenGLRenderer.h"


#include <iostream>

OpenGLRenderer::OpenGLRenderer(){
	_closed = !init();
}

OpenGLRenderer::~OpenGLRenderer() {
	shader = nullptr;
	mesh = nullptr;
	meshManager = nullptr;
	delete shader;
	delete mesh;
	delete meshManager;
}

bool OpenGLRenderer::init() {
	shader = new Shader("Shaders/vertexShaderSource.vs", "Shaders/fragmentShaderSource.fs");
	mesh = new Mesh();	
	meshManager = new ResourceManager<Mesh>;
	triangle = meshManager->put(std::string("triangle"), mesh);
	return true;
}

void OpenGLRenderer::renderPrimitive(Window *window) {
	glUseProgram(shader->ID);
	meshManager->get(triangle)->render();
	window->doubleBuffer();
}

void OpenGLRenderer::clear() {
	
}