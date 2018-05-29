#ifndef SHADER_H
#define SHADER_H

#include <glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm.hpp>

#include "LogManager.h"

// Learning material used @ learnopengl.com

class Shader
{
public:
	unsigned int ID;

	// Costructor takes in the vertex and fragment file paths
	Shader(const char* vertexPath, const char* fragmentPath);

	// Default Deconstructor
	~Shader();

	// Initialize Shader
	void Init(const char* vertexPath, const char* fragmentPath);

	// Activate the shader
	void use();

	void SetName(std::string &_name);
	
	// Utility uniform functions
	void SetBool(const std::string &name, bool value) const;
	
	void SetInt(const std::string &name, int value) const;
	
	void SetFloat(const std::string &name, float value) const;

	void SetVec3(const std::string &name, const glm::vec3 &vec) const;

	void SetMat3(const std::string &name, const glm::mat3 &mat) const;

	void SetMat4(const std::string &name, const glm::mat4 &mat) const;

	std::string GetName();

private:
	// Utility function for checking shader compilation/linking errors.

	void CheckCompileErrors(unsigned int shader, std::string type);

	std::string name;
};
#endif
