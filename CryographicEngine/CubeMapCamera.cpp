#include "CubeMapCamera.h"

CubeMapCamera::CubeMapCamera(glm::vec3 _centre) {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	pitch = 0.0f;
	yaw = 90.0f;
	FOV = 90.0f;
	aspectRatio = 1.0f;
	near = 0.1f;
	far = 100.0f;
	UpdateCameraVectors();
}

glm::mat4 CubeMapCamera::GetProjectionMatrix() {
	return glm::perspective(FOV, aspectRatio, near, far);
}

glm::mat4 CubeMapCamera::GetViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void CubeMapCamera::SwitchFace(int _index) {
	switch (_index) {
	case 0:
		pitch = 0.0f;
		yaw = 90.0f;
		break;
	case 1:
		pitch = 0.0f;
		yaw = -90.0f;
		break;
	case 2:
		pitch = -90.0f;
		yaw = 180.0f;
		break;
	case 3:
		pitch = 90.0f;
		yaw = 180.0f;
		break;
	case 4:
		pitch = 0;
		yaw = 180.0f;
		break;
	case 5:
		pitch = 0;
		yaw = 0;
		break;
	}
	UpdateCameraVectors();
}

void CubeMapCamera::UpdateCameraVectors() {
	glm::vec3 tempFront;
	tempFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	tempFront.y = sin(glm::radians(pitch));
	tempFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(tempFront);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}