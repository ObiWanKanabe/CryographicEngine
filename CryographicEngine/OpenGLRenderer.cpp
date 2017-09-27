#include "OpenGLRenderer.h"


#include <iostream>

OpenGLRenderer::OpenGLRenderer(Window *window){
	_closed = !init(window);
}

OpenGLRenderer::~OpenGLRenderer() {

}

bool OpenGLRenderer::init(Window *window) {

	shader = new Shader("Shaders/vetexShaderSource.vs", "Shaders/fragmentShaderSource.fs");
	
	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	while (true) {
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		glUseProgram(shader->ID);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		
		window->doubleBuffer();
	}
		
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		return true;
}

void OpenGLRenderer::renderPrimitive() {
	
}