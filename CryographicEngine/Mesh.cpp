#include "Mesh.h"

Mesh::Mesh() {
	vertexDescriptor = new VertexDescriptor();
	Init();
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	vertexDescriptor = nullptr;
	delete vertexDescriptor;
}

void Mesh::Init() {
}

void Mesh::AddComponent(VertexComponentDescriptor::VertexComponentType _type) {
	vertexDescriptor->AddComponent(_type);
}

void Mesh::GenerateBuffers(std::vector<GLfloat> _vertices) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), &_vertices[0], GL_STATIC_DRAW);

	for (int i = 0; i < vertexDescriptor->GetSize(); i++) {
		glVertexAttribPointer(i, vertexDescriptor->componentList[i].GetNumFloats(), GL_FLOAT, GL_FALSE, vertexDescriptor->GetStride(), (void*)(vertexDescriptor->componentList[i].offset));
		glEnableVertexAttribArray(i);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::Render() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

