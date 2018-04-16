#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <glm.hpp>
#include "Image.h"
#include "Camera.h"
#include "MeshManager.h"
#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

class Model {
public:

	// Creates a model from the file path
	Model(std::string filePath);

	// Creates a model from the file path using the provided shader
	Model(std::string filePath, Shader* shader);

	// Sets the name of the model in the manager
	void SetName(std::string &_name);

	// Gets the name of the model in the manager
	std::string GetName();

	// Gets the list of meshes in the model
	std::vector<Mesh> GetMeshes();
	
	// Binds the uniforms of the shader
	void BindUniforms(Camera *camera, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	// Function called before render
	void PreRender();

	// Renders the mesh on the screen
	void Render();

	// Function called after render
	void PostRender();

private:

	std::string name;

	// Name of the material in the manager
	std::string shaderName;

	// List of the meshes in the model
	std::vector<Mesh> meshes;
	
	std::string directory;

	std::vector<Texture> loadedTextures;

	void LoadModel(std::string filePath);

	void ProcessNode(aiNode *node, const aiScene *scene);
	
	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

	std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

};
#endif

