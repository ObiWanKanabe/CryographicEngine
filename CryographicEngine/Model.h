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

// Learning material used @ learnopengl.com

class Model {
public:

	Model() = delete;

	// Creates a model from the file path
	Model(std::string filePath);

	// Creates a model from the file path using the provided shader
	Model(std::string filePath, Shader* shader);

	// Sets the name of the model in the manager
	void SetName(std::string &_name);

	// Sets the shininess of the material
	// Suggested max of 256.0f
	void SetShininess(float _shininess);

	// The opacity of the reflection on the material
	// Between 0.0f and 1.0f
	void SetReflectiveness(float _reflectiveness);

	// Gets the name of the model in the manager
	std::string GetName();

	// Gets the models shader name in the manager
	std::string GetShaderName();

	// Gets the list of meshes in the model
	std::vector<Mesh*> GetMeshes();

	// Prints the names of the meshes loaded 
	void PrintMeshNames();
	
	// Binds the uniforms of the shader
	void BindUniforms(Camera *camera, std::vector<Light*> lights, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	// Function called before render
	void PreRender();

	// Renders the model on the screen
	void Render();

	// Draws the vertices of the model and its meshes on the GPU
	void Draw();

	// Function called after render
	void PostRender();

private:

	// Name of the model in the manager
	std::string name;

	// Name of the model's shader in the manager
	std::string shaderName;

	// List of the meshes in the model
	std::vector<Mesh*> meshes;
	
	// Directory where the model is stored
	std::string directory;

	// List of the textures in the model
	std::vector<Texture> loadedTextures;

	// Offset of the model from the drawing point
	glm::vec3 offset;

	// Initial model loading method
	void LoadModel(std::string filePath);

	// Processes the next node in the model, considering a parent-child relationship
	void ProcessNode(aiNode *node, const aiScene *scene);
	
	// Returns a mesh with all the processed parameters from ASSIMP
	Mesh* ProcessMesh(aiMesh *mesh, const aiScene *scene);

	// Loads Textures for the model
	std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

};
#endif

