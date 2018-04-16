#ifndef OBJECT_H
#define OBJECT_H

#include "BoundingVolume.h"

class Object {
public:
	Object() {};

	virtual BoundingVolume* GetBoundingVolume() = 0;
	virtual void PreRender() = 0;
	virtual void Render(Camera *camera, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) = 0;
	virtual void PostRender() = 0;
	virtual void Update(float deltaTime) = 0;
};


#endif OBJECT_H