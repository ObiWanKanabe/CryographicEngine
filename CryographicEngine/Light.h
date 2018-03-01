#ifndef LIGHT_H
#define LIGHT_H

#include "ShaderManager.h"
#include "Vertex.h"

class Light {
public:

	// A Light can have a position and a colour
	Light(glm::vec3 _pos, glm::vec3 colour);

	// A Light can have a position and a colour
	Light(float x, float y, float z, float r, float g, float b);

	// Generates the buffers for the light mesh
	void GenerateBuffers();

	// Utility function to be able to set matrices in the shader
	void SetMat4(const std::string &name, const glm::mat4 &mat);

	// Function called before render
	void PreRender();

	// Renders the mesh on the screen
	void Render();

	// Function called after render
	void PostRender();

	// Returns the Vertex Descriptor of the light
	VertexDescriptor GetVertexDescriptor() { return *vertexDescriptor; }
private:

	// VAOs and VBOs stored on the GPU
	GLuint VAO, VBO;

	// List of vertices used when creating the mesh
	std::vector<GLfloat> vertices;

	// Vertex descriptor describing how the vertices list is laid out for the GPU
	VertexDescriptor *vertexDescriptor = nullptr;
};

#endif
