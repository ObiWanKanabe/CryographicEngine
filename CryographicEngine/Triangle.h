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
	Mesh mesh;
	std::vector<GLfloat> vertices;

};
#endif