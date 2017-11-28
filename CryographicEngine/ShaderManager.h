#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "ResourceManager.h"
#include "Shader.h"

class ShaderManager {
public:
	typedef ResourceHandle<Shader> HandleType;
private:
	
	ResourceManager<Shader> shaders;
	static ShaderManager* theInstance;

	ShaderManager() {};
public:

	static ShaderManager* GetInstance();

	~ShaderManager();

	ShaderManager::HandleType StoreShader(std::string &name, const char* vertexPath, const char* fragmentPath);

	Shader* GetShader(ShaderManager::HandleType &handle);

	Shader* GetShader(std::string &name);
};
#endif

