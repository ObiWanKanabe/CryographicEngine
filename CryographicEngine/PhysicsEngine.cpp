#include "PhysicsEngine.h"

std::unique_ptr<PhysicsEngine> PhysicsEngine::theInstance(nullptr);

PhysicsEngine::~PhysicsEngine() {

}

PhysicsEngine* PhysicsEngine::GetInstance() {
	if (theInstance.get() == nullptr) {
		theInstance.reset(new PhysicsEngine());
	}
	return theInstance.get();
}

void PhysicsEngine::Update(float deltaTime) {

}

void PhysicsEngine::AddObjectList(std::vector<GameObject*> physicsObjectList) {
	objectList = physicsObjectList;
	GeneratePairs(physicsObjectList);
}

void PhysicsEngine::GeneratePairs(std::vector<GameObject*> physicsObjectList) {
	contactList.clear();
	int size = physicsObjectList.size();
	for (int i = 0; i < size; ++i) {
		for (int j = i + 1; j < size; ++j) {

			//// Checks that both objects are on the same layer
			//if (physicsObjectList.at(i)->collisionComponent->GetLayer() != physicsObjectList.at(j)->collisionComponent->GetLayer()) {
			//	continue;
			//}

			//// Checks that both objects are not static
			//if (physicsObjectList.at(i)->physicsComponent->GetPhysicsType() == PhysicsComponent::Physics_Type::STATIC &&
			//	physicsObjectList.at(j)->physicsComponent->GetPhysicsType() == PhysicsComponent::Physics_Type::STATIC) {
			//	continue;
			//}

			// Check if objects have an active physics component
			if (physicsObjectList.at(i)->GetRigidBody() == nullptr || physicsObjectList.at(j)->GetRigidBody() == nullptr) {
				continue;
			}

			// Once pair passes all tests, push into the list
			contactList.push_back(ContactData::CreateContactData(physicsObjectList.at(i), physicsObjectList.at(j)));
		}
	}
}

void PhysicsEngine::CalculateForces() {

}