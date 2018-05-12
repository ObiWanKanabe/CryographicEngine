#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm.hpp>
#include "ImageManager.h"
#include "ShaderManager.h"

// Type letting the mesh know if UV or Colour components are needed in the vertex
enum MATERIAL_TYPE {
	COLOUR,
	TEXTURE,
	MODEL_TEXTURE,
	REFLECTIVE,

};

struct Texture {
	unsigned int ID;
	std::string type;
	std::string path;
};

// Material class can have an ambient Colour and diffuse Texture (Right now)
class Material {
	
private:
	// Name of the material in the manager
	std::string name;
	
	// Type of the material
	MATERIAL_TYPE type;

	// ID of the texture in the GPU
	GLuint textureID[8];

	// The ambient colour
	glm::vec3 ambient;

	// The diffuse colour
	glm::vec3 diffuse;

	// The specular colour
	glm::vec3 specular;
	
	// The shininess of the material
	float shininess;

	// The reflectiveness of the material
	float reflectiveness;

	// Name of the texture in the manager
	std::vector<std::string> textureNames;

	// Name of the shader in the manager
	std::string shaderName;
public:

	Material() = delete;

	// Constructors can supply the basic pre-written engine shaders or you can supply your own
	Material(glm::vec3 colour);
	Material(Image *image);
	Material(std::vector<Image*> imageList);
	Material(MATERIAL_TYPE _type);
	Material(glm::vec3 colour, Shader *shader);
	Material(Image *image, Shader *shader);
	Material(std::vector<Image*> imageList, Shader *shader);
	Material(MATERIAL_TYPE _type, Shader *shader);

	// Constructor for a material loaded by assimp
	Material(std::vector<Texture> _texture, std::string &_name);

	void AddImage(Image* _image);

	// Sets the name of the material in the manager
	void SetName(std::string &_name);

	// Sets the material's shader name in the manager
	void SetShaderName(std::string &_name);

	// Setting material colours
	void SetColour(glm::vec3 _colour);
	void SetAmbientColour(glm::vec3 _ambient);
	void SetDiffuseColour(glm::vec3 _diffuse);
	void SetSpecularColour(glm::vec3 _specular);

	// The amount of shine the applied to the material
	// Suggested max of 256.0f
	void SetShininess(float _shininess);

	// The opacity of the reflection on the material
	// Between 0.0f and 1.0f
	void SetReflectiveness(float _reflectiveness);

	// Utility functions 
	std::string GetName();
	Shader* GetShader();
	MATERIAL_TYPE GetType();

	const glm::vec3 GetColour();
	const glm::vec3 GetAmbientColour();
	const glm::vec3 GetDiffuseColour();
	const glm::vec3 GetSpecularColour();
	const float GetShininess();
	const float GetReflectiveness();


	// Generation of the material(texture) on the GPU
	void Setup();

	// Binds the uniforms of the shader
	void BindUniforms();

	// Binds the uniform of the model's shader
	void BindUniforms(Shader *shader);

	// Function to be used on pre-render
	void PreRender();

	// Function to be used on render
	void Render();

	// Function to be used post-render
	void PostRender();

};
#endif
