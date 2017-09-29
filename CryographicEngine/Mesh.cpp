#include "Mesh.h"

GLfloat vertices[] =
{
	-0.5f, -0.5f, 0.0f,
	1.0f, 0.4f, 0.0f,
	0.5f, -0.5f, 0.0f,
	1.0f, 0.6f, 0.3f,
	0.0f,  0.5f, 0.0f,
	1.0f, 0.8f, 0.6f,
	
};

Mesh::Mesh() {
	vertexDescriptor = new VertexDescriptor();
	init();
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Mesh::init() {
	addComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
	addComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
	generateBuffers();
}

void Mesh::addComponent(VertexComponentDescriptor::VertexComponentType _type) {
	vertexDescriptor->addComponent(_type);
}

void Mesh::generateBuffers() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	for (int i = 0; i < vertexDescriptor->getSize(); i++) {
		glVertexAttribPointer(i, vertexDescriptor->componentList[i].getNumFloats(), GL_FLOAT, GL_FALSE, vertexDescriptor->getStride(), (void*)(vertexDescriptor->componentList[i].offset));
		glEnableVertexAttribArray(i);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::render() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

