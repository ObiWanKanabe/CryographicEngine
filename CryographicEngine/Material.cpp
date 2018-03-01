#include "Material.h"

Material::Material(glm::vec3 colour) {
	ambientColour = colour;
	shaderName = ShaderManager::GetInstance()->GetShader(std::string("defaultColour"))->GetName();
	type = MATERIAL_TYPE::COLOUR;
}

Material::Material(Image *image) {
	textureNames.push_back(image->GetName());
	shaderName = ShaderManager::GetInstance()->GetShader(std::string("defaultImage"))->GetName();
	type = MATERIAL_TYPE::TEXTURE;
}

Material::Material(std::vector<Image*> imageList) {
	for (int i = 0; i < imageList.size(); i++) {
		textureNames.push_back(imageList[i]->GetName());
	}
	shaderName = ShaderManager::GetInstance()->GetShader(std::string("defaultImage"))->GetName();
	type = MATERIAL_TYPE::TEXTURE;
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
	ambientColour = colour;
	shaderName = shader->GetName();
	type = MATERIAL_TYPE::COLOUR;
}

Material::Material(Image *image, Shader *shader) {
	textureNames.push_back(image->GetName());
	shaderName = shader->GetName();
	type = MATERIAL_TYPE::TEXTURE;
}

Material::Material(std::vector<Image*> imageList, Shader *shader) {
	for (int i = 0; i < imageList.size(); i++) {
		textureNames.push_back(imageList[i]->GetName());
	}
	shaderName = shader->GetName();
	type = MATERIAL_TYPE::TEXTURE;
}

Material::Material(MATERIAL_TYPE _type, Shader *shader) {
	if (_type = MATERIAL_TYPE::REFLECTIVE) {
		type = _type;
		shaderName = shader->GetName();
	}
	else {
		std::cerr << "Other formats not supported in this constructor yet.\n";
	}
}

void Material::SetName(std::string &_name) {
	name = _name;
}

std::string Material::GetName() {
	return name;
}

Shader* Material::GetShader() {
	return ShaderManager::GetInstance()->GetShader(shaderName);
}

glm::vec3 Material::GetColour() {
	return ambientColour;
}
MATERIAL_TYPE Material::GetType() {
	return type;
}

void Material::Setup() {
	if (type == MATERIAL_TYPE::TEXTURE) {
		for (int i = 0; i < textureNames.size(); i++) {
			Image *image = ImageManager::GetInstance()->GetImage(textureNames[i]);

			glGenTextures(textureNames.size(), textureID);
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
	shader->use();
	if (type == MATERIAL_TYPE::TEXTURE) {
		for (int i = 0; i < textureNames.size(); i++) {
			shader->SetInt(std::string("uTexture"), i);
		}
	}
	else if (type == MATERIAL_TYPE::REFLECTIVE) {
		shader->SetInt("skybox", 0);
	}
}

void Material::PreRender() {
	Shader* shader = GetShader();
	shader->use();
	if (type == MATERIAL_TYPE::TEXTURE) {
		for (int i = 0; i < textureNames.size(); i++) {
			
		}
	}
	else if (type == MATERIAL_TYPE::REFLECTIVE) {
		
	}
}

void Material::Render() {
	Shader* shader = GetShader();
	shader->use();
	if (type == MATERIAL_TYPE::TEXTURE) {
		for (int i = 0; i < textureNames.size(); i++) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureID[i]);
		}
	}
	else if (type == MATERIAL_TYPE::REFLECTIVE) {

	}
}

void Material::PostRender() {
	if (type == MATERIAL_TYPE::TEXTURE) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (type == MATERIAL_TYPE::REFLECTIVE) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}