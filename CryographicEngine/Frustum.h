#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Plane.h"
#include "BoundingVolume.h"

// Taught in Game Engine Design Class @ Humber College

// TO DO : IMPLEMENTATION / BUG FIXING

class Frustum {
private:
	enum PLANE_NAMES {
		NEARP, FARP, TOPP, BOTTOMP, RIGHTP, LEFTP
	};

	
	glm::vec3 cameraPos;
	glm::vec3 viewDir;
	float nearDist;
	float farDist;
	glm::vec3 nearCent;
	glm::vec3 farCent;
	float nearWd;
	float nearHt;
	float farWd;
	float farHt;
	glm::vec3 upDir;
	glm::vec3 rightDir;
	Plane planes[6];
	glm::vec3 ntl, nbl, nbr, ntr, ftl, fbl, fbr, ftr;
	float ratio;
	float angle;
	float tang;

public:

	enum ENCLOSURE_TYPE {
		INSIDE, OUTSIDE, OVERLAP
	};

	Frustum();
	~Frustum();

	void WindowResize(float _angle, float _ratio, float nearD, float farD);

	void CameraMovement(glm::vec3 &camPos, glm::vec3 &viewDir, glm::vec3 upDir, glm::vec3 rightDir);

	ENCLOSURE_TYPE IsInside(glm::vec3 &pt);

	ENCLOSURE_TYPE IsInside(BoundingVolume &volume);
};
#endif 

