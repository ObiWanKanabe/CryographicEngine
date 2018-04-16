#ifndef MESH_H
#define MESH_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "ShaderManager.h"
#include "MaterialManager.h"
#include "Camera.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "Vertex.h"

// List of primitive types able to be created
enum MESH_TYPE {
	PLANE,
	TRIANGLE,
	CUBE,
	MODEL
};

struct Texture {
	unsigned int ID;
	std::string type;
	std::string path;
};

// Mesh class can be loaded meshes or created primitives with the desired colour & material
class Mesh {

public:

	// Creates a mesh with the loaded vertices, indices, and textures from assimp
	Mesh(std::vector<GLfloat> _vertices, std::vector<unsigned int> _indices, std::vector <Texture> _textures);

	// Creates a mesh with the default colour
	Mesh(MESH_TYPE primType);

	// Creates a mesh with a given ambient colour
	Mesh(MESH_TYPE primType, float r, float g, float b);

	// Creates a mesh with the given material
	Mesh(MESH_TYPE primType, Material* material);

	// Default Deconstructor
	~Mesh();

	// Sets the name of the mesh in the manager
	void SetName(std::string &_name);

	// Gets the name of the mesh in the manager
	std::string GetName();

	// Adding a component to the mesh requires a type
	void AddComponent(VertexComponentDescriptor::VertexComponentType _type);

	// Generates the buffers for the mesh
	void GenerateBuffers();

	// Utility function to be able to set matrices in the shader
	void SetMat4(const std::string &name, const glm::mat4 &mat);

	// Utility function to be able to set vectors in the shader
	void SetVec3(const std::string &name, const glm::vec3 &vec);

	// Binds the uniforms of the shader
	void BindUniforms(Camera *camera, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	// Bind the uniform samplers for model meshes
	void BindUniforms(Shader *shader);

	// Function called before render
	void PreRender();

	// Renders the mesh on the screen
	void Render();

	// Function called after render
	void PostRender();

	// Returns the Vertex Descriptor of the mesh
	VertexDescriptor GetVertexDescriptor() { return vertexDescriptor; }

	std::vector<GLfloat> GetVertices();
private:
	// Name of the mesh in the manager
	std::string name;

	// Type of the mesh, often prim types or model
	MESH_TYPE type;

	// Name of the material in the manager
	std::string materialName;
	// VAOs , VBOs, and EBOs stored on the GPU
	unsigned int VAO, VBO, EBO;

	// List of vertices used when creating the mesh
	std::vector<GLfloat> vertices;

	// List of indices used when loading meshes
	std::vector<unsigned int> indices;

	// List of textures used on loaded meshes
	std::vector<Texture> textures;

private:

	// Vertex descriptor describing how the vertices list is laid out for the GPU
	VertexDescriptor vertexDescriptor;
};
#endif
