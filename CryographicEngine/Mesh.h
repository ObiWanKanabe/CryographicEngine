#ifndef MESH_H
#define MESH_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "ShaderManager.h"
#include "MaterialManager.h"
#include "Camera.h"
#include "Light.h"

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

// Mesh class can be loaded meshes or created primitives with the desired colour & material
class Mesh {

public:

	Mesh() = delete;

	// Creates a mesh with the loaded vertices, indices, and textures from assimp
	Mesh(std::vector<GLfloat> _vertices, std::vector<unsigned int> _indices, std::vector <Texture> _textures, glm::vec3 _position, std::string &_name);

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

	// Sets the shininess of the material
	// Suggested max of 256.0f
	void SetShininess(float _shininess);

	// The opacity of the reflection on the material
	// Between 0.0f and 1.0f
	void SetReflectiveness(float _reflectiveness);

	// Can repeat or stretch the UV coordinates of the texture
	void SetTextureScale(float _x, float _y);

	// Gets the name of the mesh in the manager
	std::string GetName();

	// Gets the mesh's material name in the managaer
	std::string GetMaterialName();

	// Gets the material pointer
	Material* GetMaterial();

	// Adding a component to the mesh requires a type
	void AddComponent(VertexComponentDescriptor::VertexComponentType _type);

	// Generates the buffers for the mesh
	void GenerateBuffers();

	// Binds the uniforms of the shader
	void BindUniforms(Camera *camera, std::vector<Light*> lights, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

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

	// Returns the vertices of the mesh
	std::vector<GLfloat> GetVertices();

	// Returns the offset of the vertices of the mesh
	glm::vec3 GetOffset();

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

	// The offset from the origin of the model meshes vertices
	glm::vec3 offset;

	// Vertex descriptor describing how the vertices list is laid out for the GPU
	VertexDescriptor vertexDescriptor;
};
#endif
