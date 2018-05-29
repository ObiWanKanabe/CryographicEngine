#ifndef PLANE_H
#define PLANE_H

#include <glm.hpp>

// Taught in Game Engine Design Class @ Humber College

class Plane {
private:
	glm::vec3 p;
	float a, b, c, d;
public:
	Plane();
	Plane(float _a, float _b, float _c, float _d, glm::vec3 &_p);
	Plane(glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3);
	~Plane();

	Plane& operator=(const Plane &other);

	glm::vec3 GetNormal();
	float GetDistance();
	float RelativeDistance(glm::vec3 &_p);
};
#endif
