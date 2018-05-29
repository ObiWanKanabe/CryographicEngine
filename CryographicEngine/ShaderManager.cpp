#include "ShaderManager.h"

ShaderManager* ShaderManager::theInstance = nullptr;

ShaderManager* ShaderManager::GetInstance() {
	if (theInstance == nullptr) {
		theInstance = new ShaderManager();
	}
	return theInstance;
}

ShaderManager::~ShaderManager() {
	shaders.clearKeysAndValues();
	delete theInstance;
	theInstance = nullptr;
}

ShaderManager::HandleType ShaderManager::StoreShader(std::string &name, const char* vertexPath, const char* fragmentPath) {
	Shader *shader = new Shader(vertexPath, fragmentPath);

	ShaderManager::HandleType output(-1);
	output = shaders.Get(name);
	if (!output.IsNull()) {
		std::cerr << "ERROR : Shader name: " << name << " is already in use." << std::endl;
		return output;
	}
	shader->SetName(name);
	return shaders.Put(name, shader);
}

Shader* ShaderManager::GetShader(ShaderManager::HandleType &handle) {
	Shader *output = shaders.Get(handle);
	if (output == nullptr) {
		std::cerr << "ERROR : Shader Handle is not in use." << std::endl;
	}
	return output;
}

Shader* ShaderManager::GetShader(std::string &name) {
	ShaderManager::HandleType handle = shaders.Get(name);
	if (handle.IsNull()) {
		std::cerr << "ERROR : Shader name: " << name << " is not in use." << std::endl;
	}
	Shader *output = GetShader(handle);
	return output;
}