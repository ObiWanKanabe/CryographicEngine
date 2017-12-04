#ifndef MESH_H
#define MESH_H
#include "Vertex.h"
#include "AbstractRenderer.h"
#include "ShaderManager.h"
#include "ImageManager.h"

enum PRIMITIVE_TYPE {
	PLANE,
	TRIANGLE,
	CUBE
};

class Mesh {

public:
	// Default Constructor
	Mesh(PRIMITIVE_TYPE primType);

	Mesh(PRIMITIVE_TYPE primType, float r, float g, float b);

	Mesh(PRIMITIVE_TYPE primType, Image* image);

	// Default Deconstructor
	~Mesh();

	// Initialization of the done here
	void Init();

	// Adding a component to the mesh requires a type
	void AddComponent(VertexComponentDescriptor::VertexComponentType _type);

	// Generates the buffers for the mesh
	void GenerateBuffers();

	void GenerateBuffersTexture(Image* image);

	// Renders the mesh on the screen
	void Render();

	// Returns the Vertex Descriptor of the mesh
	VertexDescriptor GetVertexDescriptor() { return *vertexDescriptor; }
private:
	bool hasTexture;
	bool hasColour;
	unsigned int textureID;
	GLuint VAO, VBO;
	std::vector<GLfloat> vertices;
	VertexDescriptor *vertexDescriptor = nullptr;
};
#endif
