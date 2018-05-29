#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <glew.h>
#include <vector>
#include "ImageManager.h"
#include "ShaderManager.h"
#include "Vertex.h"

// Learning material used @ learnopengl.com && from Scott Fielder

class CubeMap {
public:

	CubeMap() = delete;

	// Constructor takes in the list of images and uses the default skybox shader
	CubeMap(std::vector<Image*> imageList);

	// Constructor takes in the list of images and uses the provided shader
	CubeMap(std::vector<Image*> imageList, Shader* shader);

	// Utility function to be able to set matrices in the shader
	void SetMat4(const std::string &name, const glm::mat4 &mat);

	// Generation of the material(texture) on the GPU
	void SetUp();

	// Binds and Activates the texture on the GPU
	void BindTexture();

	// Binds the uniforms of the shader
	void BindUniforms(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	// Function to be used on pre-render
	void PreRender();

	// Function to be used on render
	void Render();

	// Function to be used post-render
	void PostRender();

private:

	// List of Texture names in the manager
	std::vector<std::string> textureNames;

	// List of TextureIDs
	GLuint textureID;

	// Shader Name in the manager
	std::string shaderName;

	// VAOs and VBOs stored on the GPU
	GLuint VAO, VBO;

	// List of vertices used when creating the mesh
	std::vector<GLfloat> vertices;

	// Vertex descriptor describing how the vertices list is laid out for the GPU
	VertexDescriptor *vertexDescriptor = nullptr;

};
#endif