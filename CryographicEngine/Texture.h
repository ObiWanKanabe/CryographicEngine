#ifndef TEXTURE_H
#define TEXTURE_H

#include "ImageManager.h"
#include "ShaderManager.h"
#include "Vertex.h"
#include <glew.h>
#include <vector>

class Texture {
public:
	Texture(std::vector<GLfloat> _vertices, std::vector<unsigned int> _indices, unsigned int &ID);
	~Texture();
	
	void Render(std::vector<GLfloat> _vertices, std::vector<unsigned int> _indices, unsigned int &ID);

private:
	GLuint VAO, VBO, EBO;
	VertexDescriptor *vertexDescriptor = nullptr;
};
#endif
