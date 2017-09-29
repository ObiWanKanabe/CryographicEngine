#include "OpenGLRenderer.h"


#include <iostream>

OpenGLRenderer::OpenGLRenderer(){
	_closed = !init();
}

OpenGLRenderer::~OpenGLRenderer() {
	
}

bool OpenGLRenderer::init() {
	shader = new Shader("Shaders/vertexShaderSource.vs", "Shaders/fragmentShaderSource.fs");
	mesh = new Mesh();	
	return true;
}

void OpenGLRenderer::renderPrimitive(Window *window) {
	glUseProgram(shader->ID);
	mesh->render();
	window->doubleBuffer();
}

void OpenGLRenderer::clear() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}