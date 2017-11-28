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

void OpenGLRenderer::RenderPrimitive(Window *window) {
	ShaderManager::GetInstance()->GetShader(std::string("orangeish"))->use();
	window->DoubleBuffer();
}

void OpenGLRenderer::Clear() {
	
}