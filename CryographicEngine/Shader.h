#ifndef SHADER_H
#define SHADER_H

#include <glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "LogManager.h"

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
	
	// Utility uniform functions
	void SetBool(const std::string &name, bool value) const;
	
	void SetInt(const std::string &name, int value) const;
	
	void SetFloat(const std::string &name, float value) const;

private:
	// Utility function for checking shader compilation/linking errors.

	void CheckCompileErrors(unsigned int shader, std::string type);
};
#endif
