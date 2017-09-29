#ifndef MESH_H
#define MESH_H
#include "Vertex.h"
#include "AbstractRenderer.h"
#include "Shader.h"

class Mesh {

public:
	Mesh();
	~Mesh();
	void init();
	void addComponent(VertexComponentDescriptor::VertexComponentType _type);
	void render();
	VertexDescriptor getVertexDescriptor() { return *vertexDescriptor; }
private:
	GLuint VAO, VBO;
	Shader *shader;
	VertexDescriptor *vertexDescriptor = nullptr;
};
#endif
