#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <vector>
#include <glm.hpp>
#include <gtc\quaternion.hpp>

struct RigidBody {
	float mass;
	glm::mat3 intertia;
	glm::mat3 inertiaInverse;

	float speed;

	glm::vec3 position;

	glm::vec3 velocity;
	glm::vec3 velocityBody;
	glm::vec3 angularVelocity;

	glm::vec3 acceleration;
	glm::vec3 angularAcceleration;

	glm::vec3 rotation;
	glm::quat orientation;

	glm::vec3 force;
	glm::vec3 moment;
	
	glm::mat3 momentInertiaInverse;

	float radius;
	std::vector <glm::vec3> vertexList[8];
};

#endif

