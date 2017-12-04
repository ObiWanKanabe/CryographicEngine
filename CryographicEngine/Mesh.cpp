#include "Mesh.h"

Mesh::Mesh(PRIMITIVE_TYPE primType) {
	vertexDescriptor = new VertexDescriptor();
	hasColour = false;
	hasTexture = false;
	switch (primType) {
	case PLANE:
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertices = {
			0.5f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};
		break;
	case TRIANGLE:
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertices = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f,
		};
		break;
	case CUBE:
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertices = {
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
		};
		break;
	default:
		std::cerr << "Not a valid prim type.\,";
		break;
	}
	Init();
	GenerateBuffers();
}

Mesh::Mesh(PRIMITIVE_TYPE primType, float r, float g, float b) {
	vertexDescriptor = new VertexDescriptor();
	hasColour = true;
	hasTexture = false;
	switch (primType) {
	case PLANE:
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
		vertices = {
			0.5f, 0.5f, 0.0f,   r, g, b, 
			0.5f, -0.5f, 0.0f,  r, g, b,
			-0.5f, -0.5f, 0.0f, r, g, b,
			0.5f, 0.5f, 0.0f,   r, g, b,
			-0.5f, -0.5f, 0.0f, r, g, b,
			-0.5f, 0.5f, 0.0f,  r, g, b
		};
		break;
	case TRIANGLE:
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
		vertices = {
			-0.5f, -0.5f, 0.0f, r, g, b,
			0.5f, -0.5f, 0.0f,  r, g, b,
			0.0f,  0.5f, 0.0f,  r, g, b
		};
		break;
	case CUBE:
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
		vertices = {
			-0.5f, -0.5f, -0.5f, r, g, b,
			0.5f, -0.5f, -0.5f,  r, g, b,
			0.5f,  0.5f, -0.5f,  r, g, b,
			0.5f,  0.5f, -0.5f,	 r, g, b,
			-0.5f,  0.5f, -0.5f, r, g, b,
			-0.5f, -0.5f, -0.5f, r, g, b,
								 
			-0.5f, -0.5f,  0.5f, r, g, b,
			0.5f, -0.5f,  0.5f,	 r, g, b,
			0.5f,  0.5f,  0.5f,	 r, g, b,
			0.5f,  0.5f,  0.5f,	 r, g, b,
			-0.5f,  0.5f,  0.5f, r, g, b,
			-0.5f, -0.5f,  0.5f, r, g, b,
								 
			-0.5f,  0.5f,  0.5f, r, g, b,
			-0.5f,  0.5f, -0.5f, r, g, b,
			-0.5f, -0.5f, -0.5f, r, g, b,
			-0.5f, -0.5f, -0.5f, r, g, b,
			-0.5f, -0.5f,  0.5f, r, g, b,
			-0.5f,  0.5f,  0.5f, r, g, b,
								 
			0.5f,  0.5f,  0.5f,	 r, g, b,
			0.5f,  0.5f, -0.5f,	 r, g, b,
			0.5f, -0.5f, -0.5f,	 r, g, b,
			0.5f, -0.5f, -0.5f,	 r, g, b,
			0.5f, -0.5f,  0.5f,	 r, g, b,
			0.5f,  0.5f,  0.5f,	 r, g, b,
								
			-0.5f, -0.5f, -0.5f, r, g, b,
			0.5f, -0.5f, -0.5f,	 r, g, b,
			0.5f, -0.5f,  0.5f,	 r, g, b,
			0.5f, -0.5f,  0.5f,	 r, g, b,
			-0.5f, -0.5f,  0.5f, r, g, b,
			-0.5f, -0.5f, -0.5f, r, g, b,
								
			-0.5f,  0.5f, -0.5f, r, g, b,
			0.5f,  0.5f, -0.5f,	 r, g, b,
			0.5f,  0.5f,  0.5f,	 r, g, b,
			0.5f,  0.5f,  0.5f,	 r, g, b,
			-0.5f,  0.5f,  0.5f, r, g, b,
			-0.5f,  0.5f, -0.5f, r, g, b
		};
		break;
	default:
		std::cerr << "Not a valid prim type.\,";
		break;
	}
	Init();
	GenerateBuffers();
}

Mesh::Mesh(PRIMITIVE_TYPE primType, Image* image) {
	vertexDescriptor = new VertexDescriptor();
	hasColour = false;
	hasTexture = true;
	switch (primType) {
	case PLANE:
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_UV);

		vertices = {
			0.5f,  0.5f, 0.0f,   1.0f, 0.0f,  // top right
			0.5f, -0.5f, 0.0f,   1.0f, 1.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, // bottom left
			-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, // bottom left
			-0.5f,  0.5f, 0.0f,   0.0f, 0.0f, // top left 
			0.5f,  0.5f, 0.0f,   1.0f, 0.0f   // top right
		};

		break;
	case TRIANGLE:
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_UV);
		vertices = {
			-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  // lower-left corner  
			0.5f, -0.5f, 0.0f, 1.0f, 1.0f,   // lower-right corner
			0.0f,  0.5f, 0.0f,  0.5f, 0.0f   // top-center corner
		};
		break;
	case CUBE:
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_UV);
		vertices = {
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f
		};

		break;
	default:
		std::cerr << "Not a valid prim type.\,";
		break;
	}
	Init();
	GenerateBuffersTexture(image);
}
Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	vertexDescriptor = nullptr;
	delete vertexDescriptor;
}

void Mesh::Init() {

}

void Mesh::AddComponent(VertexComponentDescriptor::VertexComponentType _type) {
	vertexDescriptor->AddComponent(_type);
}

void Mesh::GenerateBuffers() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	for (int i = 0; i < vertexDescriptor->GetSize(); i++) {
		glVertexAttribPointer(i, vertexDescriptor->componentList[i].GetNumFloats(), GL_FLOAT, GL_FALSE, vertexDescriptor->GetStride(), (void*)(vertexDescriptor->componentList[i].offset));
		glEnableVertexAttribArray(i);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::GenerateBuffersTexture(Image* image) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	for (int i = 0; i < vertexDescriptor->GetSize(); i++) {
		glVertexAttribPointer(i, vertexDescriptor->componentList[i].GetNumFloats(), GL_FLOAT, GL_FALSE, vertexDescriptor->GetStride(), (void*)(vertexDescriptor->componentList[i].offset));
		glEnableVertexAttribArray(i);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
 
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	Image* img = image;
	glTexImage2D(GL_TEXTURE_2D, 0, img->GetFormat(), img->GetWidth(), img->GetHeight(), 0, img->GetFormat(), GL_UNSIGNED_BYTE, img->GetData());
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Mesh::Render() {
	unsigned int size = vertices.size();
	if (hasColour && !hasTexture) {
		ShaderManager::GetInstance()->GetShader(std::string("orangeish"))->use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, size);
		glBindVertexArray(0);
	}
	else if (hasTexture && !hasColour) {
		ShaderManager::GetInstance()->GetShader(std::string("imageish"))->use();
		glBindTexture(GL_TEXTURE_2D, textureID);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, size);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (!hasColour && !hasTexture) {
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, size);
		glBindVertexArray(0);
	}
}

