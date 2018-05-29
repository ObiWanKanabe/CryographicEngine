#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "ContactData.h"
#include "GameObject.h"
#include <memory>

// TO DO : IMPLEMENTATION

class PhysicsEngine {
private:

	// Constructor and deconstructor is private because the physics engine is a singleton
	PhysicsEngine();
	~PhysicsEngine();

public:

	// Returns the current instance of the physics engine
	static PhysicsEngine* GetInstance();

	// Main Update function called every frame use functions on the object list as needed for the game
	void Update(float deltaTime);

	// Function adds all physics objects to the physics engine
	void AddObjectList(std::vector<GameObject*> physicsObjectList);

	// Function generates the pairs to be collided with each other based on the objectlist given
	void GeneratePairs(std::vector<GameObject*> physicsObjectList);

	// Function calculates the net force and moments of all rigidbodies
	void CalculateForces();

	// Function calculates the physics changes of each object
	void CalculatePhysics(float deltaTime);

	// Function returns an int depending on type of collision
	// No Collision : 0
	// Collision : 1
	// Penatrating : -1
	// Contact : 2
	int DetectCollision(ContactData *c);

	// Function resolves collision
	void ResolveCollision(ContactData *c);

	// Function tests AABB collision
	int AABBCollision(ContactData *c);

	// Function that tests collision with ground or other set static objects
	int CheckStaticCollision(ContactData *c);

	// Function that tests contact with ground or other set static objects
	int CheckStaticContacts(ContactData *c);

	// Basic Primitive Tests to be used when needed
	static void ClosestPointOnAABB(glm::vec3 p, glm::vec3 c, glm::vec3 r, glm::vec3 &q);

	float SqDistancePointAABB(glm::vec3 p, Box b);

	float DistanceFromPointToPlane(glm::vec3 p, glm::vec3 u, glm::vec3 v, glm::vec3 pPlane);

	bool IsPointOnFace(glm::vec3 p, std::vector<glm::vec3> plane[4]);

	glm::vec3 GetBodyZAxisVector(RigidBody r);
	glm::vec3 GetBodyXAxisVector(RigidBody r);

	glm::mat3 MakeAngularVelocityMatrix(glm::vec3 v);

	int pnpoly(int npol, std::vector<glm::vec3> vlist, glm::vec3 p);

	inline glm::vec3 QVRotate(glm::quat q, glm::vec3 v) {
		glm::vec3 result;

		result = q * v * glm::conjugate(q);

		return result;
	}

	// Singleton pointers
	static std::unique_ptr<PhysicsEngine> theInstance;
	friend std::default_delete<PhysicsEngine>;

	// List of all physics objects in the scene
	std::vector<GameObject*> objectList;

	// List of objects to collide
	std::vector<ContactData*> contactList;
};

#endif

