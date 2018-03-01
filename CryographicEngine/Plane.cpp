#include "Plane.h"

Plane::Plane() {

};

Plane::Plane(float _a, float _b, float _c, float _d, glm::vec3 &_p) {
	p = _p;
	a = _a;
	b = _b;
	c = _c;
	d = _d;
}

Plane::Plane(glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3) {
	glm::vec3 v1 = p2 - p1;
	glm::vec3 v2 = p3 - p1;
	glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
	a = normal.x;
	b = normal.y;
	c = normal.z;
	d = -(a * p1.x + b * p1.y + c * p1.z);
	p = p1;
}

Plane::~Plane() {

}

Plane& Plane::operator=(const Plane &other) {
	a = other.a;
	b = other.b;
	c = other.c;
	d = other.d;
	return *this;
}
glm::vec3 Plane::GetNormal() {
	return glm::vec3(a, b, c);
}

float Plane::GetDistance() {
	return d;
}

float Plane::RelativeDistance(glm::vec3 &_p) {
	glm::vec3 normal(a, b, c);
	return glm::dot(normal, _p) + d;
}