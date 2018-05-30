#ifndef CUBEMAPCAMERA_H
#define CUBEMAPCAMERA_H

#include "glm.hpp"
#include "glew.h"
#include <gtc/matrix_transform.hpp>

class CubeMapCamera {

public:
	CubeMapCamera(glm::vec3 _centre);

	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();

	void SwitchFace(int _index);

	void UpdateCameraVectors();


private:

	// Camera Variables
	glm::vec3 position;
	glm::vec3 front, up, right;
	glm::vec3 worldUp;

	float FOV = 90.0f;
	float aspectRatio = 1.0f;
	float near = 0.1f;
	float far = 100.0f;

	float pitch;
	float yaw;

	// Projection and View of the camera
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

};
#endif