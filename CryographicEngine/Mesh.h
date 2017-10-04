#ifndef MESH_H
#define MESH_H
#include "Vertex.h"
#include "AbstractRenderer.h"
#include "Shader.h"

class Mesh {

public:
	// Default Constructor
	Mesh();

	// Default Deconstructor
	~Mesh();

	// Initialization of the done here
	void init();

	// Adding a component to the mesh requires a type
	void addComponent(VertexComponentDescriptor::VertexComponentType _type);

	// Generates the buffers for the mesh
	void generateBuffers();

	// Renders the mesh on the screen
	void render();

	// Returns the Vertex Descriptor of the mesh
	VertexDescriptor getVertexDescriptor() { return *vertexDescriptor; }
private:
	GLuint VAO, VBO;
	VertexDescriptor *vertexDescriptor = nullptr;
};
#endif
