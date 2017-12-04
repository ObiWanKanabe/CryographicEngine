#include "Texture.h"

Texture::Texture(std::vector<GLfloat> _vertices, std::vector<unsigned int> _indices, unsigned int &ID) {

	vertexDescriptor = new VertexDescriptor();

	vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
	//vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
	vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_UV);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), &_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLfloat), &_indices[0], GL_STATIC_DRAW);

	for (int i = 0; i < vertexDescriptor->GetSize(); i++) {
		glVertexAttribPointer(i, vertexDescriptor->componentList[i].GetNumFloats(), GL_FLOAT, GL_FALSE, vertexDescriptor->GetStride(), (void*)(vertexDescriptor->componentList[i].offset));
		glEnableVertexAttribArray(i);
	}
	
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	Image* img = ImageManager::GetInstance()->GetImage(std::string("derp"));
	glTexImage2D(GL_TEXTURE_2D, 0, img->GetFormat(), img->GetWidth(), img->GetHeight(), 0, img->GetFormat(), GL_UNSIGNED_BYTE, img->GetData());
	glGenerateMipmap(GL_TEXTURE_2D);
	
}

Texture::~Texture() {

}

void Texture::Render(std::vector<GLfloat> _vertices, std::vector<unsigned int> _indices, unsigned int &ID) {
	glBindTexture(GL_TEXTURE_2D, ID);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, _vertices.size(), GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}