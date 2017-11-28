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
	void Init();

	// Adding a component to the mesh requires a type
	void AddComponent(VertexComponentDescriptor::VertexComponentType _type);

	// Generates the buffers for the mesh
	void GenerateBuffers(std::vector<GLfloat> _vertices);

	// Renders the mesh on the screen
	void Render(std::vector<GLfloat> _vertices);

	// Returns the Vertex Descriptor of the mesh
	VertexDescriptor GetVertexDescriptor() { return *vertexDescriptor; }
private:
	GLuint VAO, VBO;
	VertexDescriptor *vertexDescriptor = nullptr;
};
#endif
