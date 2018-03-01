#ifndef CONTACTDATA_H
#define CONTACTDATA_H

#include "GameObject.h"

struct ContactData {
	GameObject *A;
	GameObject *B;
	glm::vec3 normal;
	glm::vec3 point;
	glm::vec3 tangent;
	glm::vec3 relativeVelocity;
	glm::vec3 relativeAcceleration;

	static ContactData * CreateContactData(GameObject *_A, GameObject *_B) {
		ContactData *contact = new ContactData();
		contact->A = _A;
		contact->B = _B;
		return contact;
	}
};
#endif