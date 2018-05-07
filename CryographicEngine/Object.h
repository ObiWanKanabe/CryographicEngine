#ifndef OBJECT_H
#define OBJECT_H

#include "BoundingVolume.h"
#include "Light.h"

class Object {
public:
	Object() {};

	virtual BoundingVolume* GetBoundingVolume() = 0;
	virtual Light* GetAttachedLight() = 0;
	virtual void SetModelMatrix(glm::mat4 _model) = 0;
	virtual glm::mat4 GetModelMatrix() = 0;
	virtual void PreRender() = 0;
	virtual void Render(Camera *camera, std::vector<Light*> lights, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) = 0;
	virtual void Render(Shader *shader) = 0;
	virtual void PostRender() = 0;
	virtual void Update(float deltaTime) = 0;
};


#endif OBJECT_H