#include "Frustum.h"
#include "BoundingVolume.h"

#define ANG2RAD 3.14159265358979323846/180.0
Frustum::Frustum() {

}

Frustum::~Frustum() {

}

void Frustum::WindowResize(float _angle, float _ratio, float nearD, float farD) {
	angle = _angle;
	ratio = _ratio;
	nearDist = nearD;
	farDist = farD;

	tang = (float)tan(ANG2RAD * angle * 0.5);
	nearHt = nearDist * tang;
	nearWd = nearHt * ratio;
	farHt = farDist * tang;
	farWd = farHt * ratio;
}

void Frustum::CameraMovement(glm::vec3 &_camPos, glm::vec3 &_viewDir, glm::vec3 _upDir, glm::vec3 _rightDir) {
	
	nearCent = _camPos - (_viewDir * nearDist);
	farCent = _camPos - (_viewDir * farDist);

	nearCent = _camPos - (_viewDir * nearDist);
	farCent = _camPos - (_viewDir * farDist);

	ntl = nearCent + _upDir * nearHt - _rightDir * nearWd;
	ntr = nearCent + _upDir * nearHt + _rightDir * nearWd;
	nbl = nearCent - _upDir * nearHt - _rightDir * nearWd;
	nbr = nearCent - _upDir * nearHt + _rightDir * nearWd;

	ftl = farCent + _upDir * farHt - _rightDir * farWd;
	ftr = farCent + _upDir * farHt + _rightDir * farWd;
	fbl = farCent - _upDir * farHt - _rightDir * farWd;
	fbr = farCent - _upDir * farHt + _rightDir * farWd;

	planes[TOPP] = Plane(ntr, ntl, ftl);
	planes[BOTTOMP] = Plane(nbl, nbr, fbr);
	planes[LEFTP] = Plane(ntl, nbl, fbl);
	planes[RIGHTP] = Plane(nbr, ntr, fbr);
	planes[NEARP] = Plane(ntl, ntr, nbr);
	planes[FARP] = Plane(ftr, ftl, fbl);
}

Frustum::ENCLOSURE_TYPE  Frustum::IsInside(glm::vec3 &pt) {
	for (int i = 0; i < 6; i++)
	{
		if (planes[i].RelativeDistance(pt) < 0)
		{
			return OUTSIDE;
		}
	}
	return INSIDE;
}

Frustum::ENCLOSURE_TYPE  Frustum::IsInside(BoundingVolume &volume) {
	if (volume.GetShape() == BOUNDING_SHAPE::SPHERE) {
		float distance;
		ENCLOSURE_TYPE result = INSIDE;

		for (int i = 0; i < 6; i++) {
			distance = planes[i].RelativeDistance(volume.GetBoundingSphere()->c);
			if (distance < -volume.GetBoundingSphere()->r) return OUTSIDE;
			else if (distance < volume.GetBoundingSphere()->r) result = OVERLAP;
		}
		return result;
	}
	else if (volume.GetShape() == BOUNDING_SHAPE::AABB) {
		ENCLOSURE_TYPE result = INSIDE;

		for (int i = 0; i < 6; i++) {
			if (planes[i].RelativeDistance(volume.GetPositiveVertex(planes[i].GetNormal())) < 0)
				return OUTSIDE;
			else if (planes[i].RelativeDistance(volume.GetNegativeVertex(planes[i].GetNormal())) < 0)
				return OVERLAP;
		}
		return result;
	}
}