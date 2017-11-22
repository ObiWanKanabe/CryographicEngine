#include "OpenGLRenderer.h"


#include <iostream>

OpenGLRenderer::OpenGLRenderer(){
	_closed = !Init();
}

OpenGLRenderer::~OpenGLRenderer() {
}

bool OpenGLRenderer::Init() {
	shaderManager = new ShaderManager<Shader>;
	return true;
}

void OpenGLRenderer::RenderPrimitive(Window *window) {
	shaderManager->Get(shaderManager->Get(std::string("orangeish")))->use();
	window->DoubleBuffer();
}

void OpenGLRenderer::Clear() {
	
}