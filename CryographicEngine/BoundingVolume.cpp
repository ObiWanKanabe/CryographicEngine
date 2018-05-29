#include "BoundingVolume.h"

BoundingVolume::BoundingVolume(BOUNDING_SHAPE _type, Mesh *mesh) {
	type = _type;
	std::vector<GLfloat> vertexList = mesh->GetVertices();
	bool firstSet = true;
	glm::vec3 min, max;
	int componentListSize = mesh->GetVertexDescriptor().GetSize();
	for (int i = 0; i < componentListSize; i++) {
		VertexComponentDescriptor vertexDesc = mesh->GetVertexDescriptor().GetComponentList()[i];
		if (vertexDesc.type == VertexComponentDescriptor::VERTEX_POSITION) {
			int offset = vertexDesc.offset / sizeof(float);
			int inc = mesh->GetVertexDescriptor().GetStride() / sizeof(float);
			int size = vertexList.size() / inc;
			for (int j = 0; j < size; j++) {
				if (firstSet) {
					max[0] = vertexList[offset];
					max[1] = vertexList[offset + 1];
					max[2] = vertexList[offset + 2];
					min[0] = vertexList[offset];
					min[1] = vertexList[offset + 1];
					min[2] = vertexList[offset + 2];
					firstSet = false;
				}
				else {
					if (vertexList[offset] > max[0]) max[0] = vertexList[offset];
					if (vertexList[offset + 1] > max[1]) max[1] = vertexList[offset + 1];
					if (vertexList[offset + 2] > max[2]) max[2] = vertexList[offset + 2];

					if (vertexList[offset] < min[0]) min[0] = vertexList[offset];
					if (vertexList[offset + 1] < min[1]) min[1] = vertexList[offset + 1];
					if (vertexList[offset + 2] < min[2]) min[2] = vertexList[offset + 2];
				}
				offset += inc;
			}
			min += mesh->GetOffset();
			max += mesh->GetOffset();
		}
	}
	if (type == BOUNDING_SHAPE::AABB) {
		BoundingAABB = new Box();
	}
	else if (type == BOUNDING_SHAPE::SPHERE) {
		BoundingSphere = new Sphere();
	}
	else if (type == BOUNDING_SHAPE::OBB) {
		// In Progress
	}
	SetCentreFromValues(min, max);
	SetRadiiFromValues(min, max);

}

BoundingVolume::BoundingVolume(BOUNDING_SHAPE _type, Model *model) {
	type = _type;
	glm::vec3 min, max;

	for (size_t j = 0; j < model->GetMeshes().size(); j++) {
		std::vector<GLfloat> vertexList = model->GetMeshes()[j]->GetVertices();
		int componentListSize = model->GetMeshes()[j]->GetVertexDescriptor().GetSize();
		for (int i = 0; i < componentListSize; i++) {
			bool firstSet = true;
			VertexComponentDescriptor vertexDesc = model->GetMeshes()[j]->GetVertexDescriptor().GetComponentList()[i];
			if (vertexDesc.type == VertexComponentDescriptor::VERTEX_POSITION) {
				int offset = vertexDesc.offset / sizeof(float);
				int inc = model->GetMeshes()[j]->GetVertexDescriptor().GetStride() / sizeof(float);
				int size = vertexList.size() / inc;
				for (int j = 0; j < size; j++) {
					if (firstSet) {
						max[0] = vertexList[offset];
						max[1] = vertexList[offset + 1];
						max[2] = vertexList[offset + 2];
						min[0] = vertexList[offset];
						min[1] = vertexList[offset + 1];
						min[2] = vertexList[offset + 2];
						firstSet = false;
					}
					else {
						if (vertexList[offset] > max[0]) max[0] = vertexList[offset];
						if (vertexList[offset + 1] > max[1]) max[1] = vertexList[offset + 1];
						if (vertexList[offset + 2] > max[2]) max[2] = vertexList[offset + 2];

						if (vertexList[offset] < min[0]) min[0] = vertexList[offset];
						if (vertexList[offset + 1] < min[1]) min[1] = vertexList[offset + 1];
						if (vertexList[offset + 2] < min[2]) min[2] = vertexList[offset + 2];
					}
					offset += inc;
				}
			}
		}
	}
	if (type == BOUNDING_SHAPE::AABB) {
		BoundingAABB = new Box();
	}
	else if (type == BOUNDING_SHAPE::SPHERE) {
		BoundingSphere = new Sphere();
	}
	else if (type == BOUNDING_SHAPE::OBB) {
		// In Progress
	}
	SetCentreFromValues(min, max);
	SetRadiiFromValues(min, max);

}

BoundingVolume::BoundingVolume() {


}

inline void BoundingVolume::SetCentreFromValues(glm::vec3& min, glm::vec3& max) {
	if (type == BOUNDING_SHAPE::AABB) {
		BoundingAABB->c = glm::vec3((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f);
	}
	else if (type == BOUNDING_SHAPE::SPHERE) {
		BoundingSphere->c = (min + max) / 2.0f;
	}
	else if (type == BOUNDING_SHAPE::OBB) {
		// In Progress
	}
}

inline void BoundingVolume::SetRadiiFromValues(glm::vec3& min, glm::vec3& max) {
	if (type == BOUNDING_SHAPE::AABB) {
		for (int i = 0; i < 3; i++) {
			BoundingAABB->r[i] = ((max[i] - min[i]) / 2.0f);
		}
	}
	else if (type == BOUNDING_SHAPE::SPHERE) {
		BoundingSphere->r = glm::length(max - min) / 2.0f;
	}
	else if (type == BOUNDING_SHAPE::OBB) {
		// In Progress
	}
}

BOUNDING_SHAPE BoundingVolume::GetShape() {
	return type;
}

Box* BoundingVolume::GetBoundingBox() {
	return BoundingAABB;
}

Sphere* BoundingVolume::GetBoundingSphere() {
	return BoundingSphere;
}

void BoundingVolume::SetPosition(glm::vec3 pos) {
	if (type == BOUNDING_SHAPE::AABB) {
		BoundingAABB->c = pos;
	}
	else if (type == BOUNDING_SHAPE::SPHERE) {
		BoundingSphere->c = pos;
	}
}

glm::vec3 BoundingVolume::GetMaximumCorner() {
	if (type = BOUNDING_SHAPE::AABB)
		return BoundingAABB->c + BoundingAABB->r;
	else
		return glm::vec3(0.0f);
}

glm::vec3 BoundingVolume::GetMinimumCorner() {
	if (type = BOUNDING_SHAPE::AABB)
		return BoundingAABB->c - BoundingAABB->r;
	else
		return glm::vec3(0.0f);
}

glm::vec3 BoundingVolume::GetPositiveVertex(glm::vec3 &normal) {
	if (type = BOUNDING_SHAPE::AABB) {
		glm::vec3 minCorner = BoundingAABB->c - BoundingAABB->r;
		glm::vec3 maxCorner = BoundingAABB->c + BoundingAABB->r;
		glm::vec3 result(minCorner);

		if (normal.x > 0) result.x = maxCorner.x;
		if (normal.y > 0) result.y = maxCorner.y;
		if (normal.z > 0) result.z = maxCorner.z;

		return result;
	}
	else {
		return glm::vec3(0.0);
	}
}

glm::vec3 BoundingVolume::GetNegativeVertex(glm::vec3 &normal) {
	if (type = BOUNDING_SHAPE::AABB) {
		glm::vec3 minCorner = BoundingAABB->c - BoundingAABB->r;
		glm::vec3 maxCorner = BoundingAABB->c + BoundingAABB->r;
		glm::vec3 result(minCorner);

		if (normal.x < 0) result.x = maxCorner.x;
		if (normal.y < 0) result.y = maxCorner.y;
		if (normal.z < 0) result.z = maxCorner.z;

		return result;
	}
	else {
		return glm::vec3(0.0);
	}
}
