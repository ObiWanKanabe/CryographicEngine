#include "Camera.h"

Camera::Camera() {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = -90.0f;
	pitch = 0.0f;
	movementSpeed = 2.5f;
	mouseSensitivity = 0.1f;
	FOV = 75.0f;
	near_plane = 0.1f;
	far_plane = 100.0f;
	/*frustum = new Frustum();
	frustum->WindowResize(FOV, 1200.0f / 900.0f, 0.1f, 100.0f);*/
	UpdateCameraVectors();
}

Camera::Camera(glm::vec3 _pos, float _yaw, float _pitch) : position(_pos), yaw(-90.0f + _yaw), pitch(_pitch) {
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	movementSpeed = 2.5f;
	mouseSensitivity = 0.1f;
	FOV = 75.0f;
	near_plane = 0.1f;
	far_plane = 100.0f;
	/*frustum = new Frustum();
	frustum->WindowResize(FOV, 1200.0f / 900.0f, 0.1f, 100.0f);*/
	UpdateCameraVectors();
}

Camera::Camera(glm::vec3 _pos, float _yaw, float _pitch, float _speed, float _sensitivity, float _zoom) :
	position(_pos), yaw(-90.0f + _yaw), pitch(_pitch), movementSpeed(_speed), mouseSensitivity(_sensitivity), FOV(_zoom) {	
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	FOV = 75.0f;
	near_plane = 0.1f;
	far_plane = 100.0f;
	/*frustum = new Frustum();
	frustum->WindowResize(FOV, 1200.0f / 900.0f, 0.1f, 100.0f);
	frustum->CameraMovement(position, -front, up, right);*/
	UpdateCameraVectors();
}

//Frustum *Camera::GetFrustum() {
//	return frustum;
//}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::GetPosition() {
	return position;
}

glm::vec3 Camera::GetFront() {
	return front;
}

glm::vec3 Camera::GetUp() {
	return up;
}

glm::vec3 Camera::GetRight() {
	return right;
}

float Camera::GetFOV() {
	return FOV;
}

float Camera::GetNear() {
	return near_plane;
}

float Camera::GetFar() {
	return far_plane;
}

void Camera::ProcessKeyboard(CAMERA_DIRECTION dir, float deltaTime) {
	float velocity = movementSpeed * deltaTime;
	if (dir == FORWARD)
		position += front * velocity;
	if (dir == BACKWARD)
		position -= front * velocity;
	if (dir == LEFT)
		position -= right * velocity;
	if (dir == RIGHT)
		position += right * velocity;

	/*frustum->CameraMovement(position, -front, up, right);*/
}

void Camera::ProcessMouseMovement(float eventX, float eventY) {
	bool constrainPitch = true;
	if (firstMouse)
	{
		lastX = eventX;
		lastY = eventY;
		firstMouse = false;
	}

	float offsetX = eventX - lastX;
	float offsetY = lastY - eventY;

	lastX = eventX;
	lastY = eventY;

	offsetX *= mouseSensitivity;
	offsetY *= mouseSensitivity;

	yaw += offsetX;
	pitch += offsetY;

	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
	/*frustum->CameraMovement(position, -front, up, right);*/
	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float offsetY) {
	if (FOV >= 10.0f && FOV <= 75.0f)
		FOV -= offsetY;
	if (FOV <= 10.0f)
		FOV = 10.0f;
	if (FOV >= 75.0f)
		FOV = 75.0f;
}

void Camera::UpdateCameraVectors() {
	glm::vec3 tempFront;
	tempFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	tempFront.y = sin(glm::radians(pitch));
	tempFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(tempFront);
	
	right = glm::normalize(glm::cross(front, worldUp));  
	up = glm::normalize(glm::cross(right, front));
}

void Camera::SetRotationX(float _pitch) {
	pitch = _pitch;
	UpdateCameraVectors();
}

void Camera::SetRotationY(float _yaw) {
	yaw = _yaw;
	UpdateCameraVectors();
}

void Camera::SetPosition(glm::vec3 _pos) {
	position = _pos;
}