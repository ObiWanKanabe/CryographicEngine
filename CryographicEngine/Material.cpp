#include "Material.h"

Material::Material(glm::vec3 colour) {
	SetColour(colour);
	shaderName = ShaderManager::GetInstance()->GetShader(std::string("defaultColour"))->GetName();
	type = MATERIAL_TYPE::COLOUR;
}

Material::Material(Image *image) {
	SetAmbientColour(glm::vec3(0.0f));
	SetDiffuseColour(glm::vec3(0.0f));
	SetSpecularColour(glm::vec3(1.0f));
	textureNames.push_back(image->GetName());
	shaderName = ShaderManager::GetInstance()->GetShader(std::string("defaultImage"))->GetName();
	shininess = 32.0f;
	reflectiveness = 0.0f;
	type = MATERIAL_TYPE::TEXTURE;

	for (size_t i = 0; i < textureNames.size(); i++) {
		if (ImageManager::GetInstance()->GetImage(textureNames[i])->GetMapType() == MAP_TYPE::NORMAL) {
			shaderName = ShaderManager::GetInstance()->GetShader(std::string("defaultImageNormals"))->GetName();
		}
	}
}

Material::Material(std::vector<Image*> imageList) {
	SetAmbientColour(glm::vec3(0.0f));
	SetDiffuseColour(glm::vec3(0.0f));
	SetSpecularColour(glm::vec3(1.0f));
	for (int i = 0; i < imageList.size(); i++) {
		textureNames.push_back(imageList[i]->GetName());
	}
	shaderName = ShaderManager::GetInstance()->GetShader(std::string("defaultImage"))->GetName();
	shininess = 32.0f;
	reflectiveness = 0.0f;
	type = MATERIAL_TYPE::TEXTURE;

	for (size_t i = 0; i < textureNames.size(); i++) {
		if (ImageManager::GetInstance()->GetImage(textureNames[i])->GetMapType() == MAP_TYPE::NORMAL) {
			shaderName = ShaderManager::GetInstance()->GetShader(std::string("defaultImageNormals"))->GetName();
		}
	}
}

Material::Material(MATERIAL_TYPE _type) {
	if (_type = MATERIAL_TYPE::REFLECTIVE) {
		type = _type;
		shaderName = ShaderManager::GetInstance()->GetShader(std::string("defaultReflection"))->GetName();
	}
	else {
		std::cerr << "Other formats not supported in this constructor yet.\n";
	}
}

Material::Material(glm::vec3 colour, Shader *shader) {
	SetColour(colour);
	if (shader->GetName() == "" && name == "") {
		std::cerr << "Shader provided for coloured material is not in the shader manager and cannot be used" << std::endl;
	}
	else if (shader->GetName() == "") {
		std::cerr << "Shader provided for material: " << name << " is not in the shader manager and cannot be used" << std::endl;
	}
	shaderName = shader->GetName();
	type = MATERIAL_TYPE::COLOUR;
}

Material::Material(Image *image, Shader *shader) {

	if (shader->GetName() == "" && name == "") {
		std::cerr << "Shader provided for textured material is not in the shader manager and cannot be used" << std::endl;
	}
	else if (shader->GetName() == "") {
		std::cerr << "Shader provided for: " << name << " is not in the shader manager and cannot be used" << std::endl;
	}

	if (image->GetName() == "" && name == "") {
		std::cerr << "Image provided for model is not in the image manager and cannot be used" << std::endl;
	}
	else if (image->GetName() == "") {
		std::cerr << "Image provided for: " << name << " is not in the image manager and cannot be used" << std::endl;
	}

	textureNames.push_back(image->GetName());
	shaderName = shader->GetName();
	SetAmbientColour(glm::vec3(0.0f));
	SetDiffuseColour(glm::vec3(0.0f));
	SetSpecularColour(glm::vec3(1.0f));
	shininess = 32.0f;
	reflectiveness = 0.0f;
	type = MATERIAL_TYPE::TEXTURE;
}

Material::Material(std::vector<Image*> imageList, Shader *shader) {
	if (shader->GetName() == "" && name == "") {
		std::cerr << "Shader provided for textured material is not in the shader manager and cannot be used" << std::endl;
	}
	else if (shader->GetName() == "") {
		std::cerr << "Shader provided for: " << name << " is not in the shader manager and cannot be used" << std::endl;
	}

	for (int i = 0; i < imageList.size(); i++) {
		if (imageList[i]->GetName() == "" && name == "") {
			std::cerr << "Images provided for texture material are not in the image manager and cannot be used" << std::endl;
		}
		else if (imageList[i]->GetName() == "") {
			std::cerr << "Images provided for: " << name << " are not in the image manager and cannot be used" << std::endl;
		}
	}

	for (int i = 0; i < imageList.size(); i++) {
		textureNames.push_back(imageList[i]->GetName());
	}
	shaderName = shader->GetName();
	SetAmbientColour(glm::vec3(0.0f));
	SetDiffuseColour(glm::vec3(0.0f));
	SetSpecularColour(glm::vec3(1.0f));
	shininess = 32.0f;
	reflectiveness = 0.0f;
	type = MATERIAL_TYPE::TEXTURE;
}

Material::Material(MATERIAL_TYPE _type, Shader *shader) {
	if (shader->GetName() == "" && name == "") {
		std::cerr << "Shader provided for material is not in the shader manager and cannot be used" << std::endl;
	}
	else if (shader->GetName() == "") {
		std::cerr << "Shader provided for: " << name << " is not in the shader manager and cannot be used" << std::endl;
	}

	if (_type = MATERIAL_TYPE::REFLECTIVE) {
		type = _type;
		shaderName = shader->GetName();
	}
	else {
		std::cerr << "Other formats not supported in this constructor yet.\n";
	}
	shininess = 32.0f;
	reflectiveness = 0.0f;
}

Material::Material(std::vector<Texture> _texture, std::string &_name) {
	shininess = 32.0f;
	reflectiveness = 0.0f;
	for (size_t i = 0; i < _texture.size(); i++) {
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int reflectNr = 1;
		textureID[i] = _texture[i].ID;
		type = MATERIAL_TYPE::MODEL_TEXTURE;
		if (_texture[i].type == "texture_diffuse") {
			textureNames.push_back("material.diffuse" + std::to_string(diffuseNr++));
		}
		else if (_texture[i].type == "texture_specular") {
			textureNames.push_back("material.specular" + std::to_string(specularNr++));
		}
		else if (_texture[i].type == "texture_normal") {
			textureNames.push_back("material.normal" + std::to_string(normalNr++));
		}
		else if (_texture[i].type == "texture_reflective") {
			textureNames.push_back("material.reflective" + std::to_string(reflectNr++));
			reflectiveness = 1.0f;
		}
		else {

		}
	}
}

void Material::AddImage(Image* _image) {
	textureNames.push_back(_image->GetName());
	if (_image->GetMapType() == MAP_TYPE::NORMAL) {
		shaderName = ShaderManager::GetInstance()->GetShader(std::string("defaultImageNormals"))->GetName();
	}
	
}

void Material::SetName(std::string &_name) {
	name = _name;
}

void Material::SetShaderName(std::string &_name) {
	shaderName = _name;
}

void Material::SetColour(glm::vec3 _colour) {
	ambient = _colour;
	diffuse = _colour;
	specular = glm::vec3(0.5f, 0.5f, 0.5f);
	shininess = 32.0f;
	reflectiveness = 0.0f;
}
void Material::SetAmbientColour(glm::vec3 _ambient) {
	ambient = _ambient;
}

void Material::SetDiffuseColour(glm::vec3 _diffuse) {
	diffuse = _diffuse;
}

void Material::SetSpecularColour(glm::vec3 _specular) {
	specular = _specular;
}

void Material::SetShininess(float _shininess) {
	shininess = glm::clamp(_shininess, 0.0f, 256.0f);
}

void Material::SetReflectiveness(float _reflectiveness) {
	reflectiveness = glm::clamp(_reflectiveness, 0.0f, 1.0f);
}

std::string Material::GetName() {
	return name;
}

Shader* Material::GetShader() {
	return ShaderManager::GetInstance()->GetShader(shaderName);
}

MATERIAL_TYPE Material::GetType() {
	return type;
}

const glm::vec3 Material::GetColour() {
	return diffuse;
}

const glm::vec3 Material::GetAmbientColour() {
	return ambient;
}

const glm::vec3 Material::GetDiffuseColour() {
	return diffuse;
}

const glm::vec3 Material::GetSpecularColour() {
	return specular;
}

const float Material::GetShininess() {
	return shininess;
}

const float Material::GetReflectiveness() {
	return reflectiveness;
}

void Material::Setup() {
	if (type == MATERIAL_TYPE::TEXTURE) {
		for (int i = 0; i < textureNames.size(); i++) {
			Image *image = ImageManager::GetInstance()->GetImage(textureNames[i]);

			glGenTextures(textureNames.size(), &textureID[i]);
			glBindTexture(GL_TEXTURE_2D, textureID[i]);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, image->GetFormat(), image->GetWidth(), image->GetHeight(), 0, image->GetFormat(), GL_UNSIGNED_BYTE, image->GetData());
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

void Material::BindUniforms() {
	Shader* shader = GetShader();
	shader->Use();
	shader->SetInt("material.diffuse1", 0);
	shader->SetInt("material.specular1", 0);
	shader->SetInt("material.normal1", 0);
	shader->SetInt("material.reflection1", 0);
	if (type == MATERIAL_TYPE::TEXTURE) {
		for (int i = 0; i < textureNames.size(); i++) {
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int reflectNr = 1;
			if (ImageManager::GetInstance()->GetImage(textureNames[i])->GetMapType() == MAP_TYPE::DIFFUSE) {
				shader->SetInt("material.diffuse" + std::to_string(diffuseNr++), i);
			}
			else if (ImageManager::GetInstance()->GetImage(textureNames[i])->GetMapType() == MAP_TYPE::SPECULAR) {
				shader->SetInt("material.specular" + std::to_string(specularNr++), i);
			}
			else if (ImageManager::GetInstance()->GetImage(textureNames[i])->GetMapType() == MAP_TYPE::NORMAL) {
				shader->SetInt("material.normal" + std::to_string(normalNr++), i);
			}
			else if (ImageManager::GetInstance()->GetImage(textureNames[i])->GetMapType() == MAP_TYPE::REFLECTION) {
				shader->SetInt("material.reflection" + std::to_string(reflectNr++), i);
			}
			else {
				shader->SetInt("material.diffuse" + std::to_string(diffuseNr++), i);
			}
		}
	}
	else if (type == MATERIAL_TYPE::MODEL_TEXTURE) {
		for (unsigned int i = 0; i < textureNames.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);

				shader->SetInt(textureNames[i], i);
			}

		if (textureNames.size() == 1) {
			shader->SetInt("texture_specular1", 0);
			shader->SetInt("texture_normal1", 0);
		}
	}
	else if (type == MATERIAL_TYPE::REFLECTIVE) {
		shader->SetInt("skybox", 5);
	}
	shader->SetVec3("material.ambient", ambient);
	shader->SetVec3("material.diffuse", diffuse);
	shader->SetVec3("material.specular", specular);
	shader->SetFloat("material.shininess", shininess);
	shader->SetFloat("material.reflectiveness", reflectiveness);
}

void Material::BindUniforms(Shader *shader) {
	shader->Use();
	for (unsigned int i = 0; i < textureNames.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		shader->SetInt(textureNames[i], i);
		shader->SetInt("skybox", 5);
	}
	if (textureNames.size() == 1) {
		shader->SetInt("texture_specular1", 0);
		shader->SetInt("texture_normal1", 0);
	}
	shader->SetVec3("material.ambient", ambient);
	shader->SetVec3("material.diffuse", diffuse);
	shader->SetVec3("material.specular", specular);
	shader->SetFloat("material.shininess", shininess);
	shader->SetFloat("material.reflectiveness", reflectiveness);
}

void Material::PreRender() {
	Shader* shader = GetShader();
	//shader->use();
	if (type == MATERIAL_TYPE::TEXTURE) {
		for (int i = 0; i < textureNames.size(); i++) {
			
		}
	}
	else if (type == MATERIAL_TYPE::REFLECTIVE) {
		
	}
}

void Material::Render() {
	if (type == MATERIAL_TYPE::TEXTURE || type == MATERIAL_TYPE::MODEL_TEXTURE) {
		for (int i = 0; i < textureNames.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textureID[i]);
		}
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (type == MATERIAL_TYPE::REFLECTIVE) {

	}
}

void Material::PostRender() {
	/*if (type == MATERIAL_TYPE::MODEL_TEXTURE) {
		glBindTexture(GL_TEXTURE_2D, 0);
	} 
	else if (type == MATERIAL_TYPE::TEXTURE) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (type == MATERIAL_TYPE::REFLECTIVE) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}*/
}