#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include <glm.hpp>
#include "MeshManager.h"
#include "ModelManager.h"

enum BOUNDING_SHAPE {
	AABB,
	SPHERE,
	OBB
};

struct Box {
	glm::vec3 c;
	glm::vec3 r;
};

struct Sphere {
	glm::vec3 c;
	float r;
};

class BoundingVolume {
private:
	Box *BoundingAABB;
	Sphere *BoundingSphere;
	BOUNDING_SHAPE type;
public:
	BoundingVolume(BOUNDING_SHAPE _type, Mesh *mesh);
	BoundingVolume(BOUNDING_SHAPE _type, Model* model);
	BoundingVolume();

	inline void SetCentreFromValues(glm::vec3& min, glm::vec3& max);
	inline void SetRadiiFromValues(glm::vec3& min, glm::vec3& max);

	BOUNDING_SHAPE GetShape();
	Box* GetBoundingBox();
	Sphere* GetBoundingSphere();

	void SetPosition(glm::vec3 pos);

	glm::vec3 GetMaximumCorner();
	glm::vec3 GetMinimumCorner();

	glm::vec3 GetPositiveVertex(glm::vec3 &normal);
	glm::vec3 GetNegativeVertex(glm::vec3 &normal);
};

#endif
