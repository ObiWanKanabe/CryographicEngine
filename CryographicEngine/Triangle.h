#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "GameObject.h"

class Triangle : public GameObject {
public:
	Triangle();
	~Triangle();
	void Update();
	void Render();
private:
	unsigned int textureID;
	Texture* texture;
	Mesh* mesh;
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

};
#endif