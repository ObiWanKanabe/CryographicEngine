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
}

ShaderManager::HandleType ShaderManager::StoreShader(std::string &name, const char* vertexPath, const char* fragmentPath) {
	Shader *shader = new Shader(vertexPath, fragmentPath);
	return shaders.Put(name, shader);
}

Shader* ShaderManager::GetShader(ShaderManager::HandleType &handle) {
	Shader *output = shaders.Get(handle);
	return output;
}

Shader* ShaderManager::GetShader(std::string &name) {
	ShaderManager::HandleType handle = shaders.Get(name);
	Shader *output = GetShader(handle);
	return output;
}