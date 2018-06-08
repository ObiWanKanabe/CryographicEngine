#ifndef CAMERA_H
#define CAMERA_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include "Window.h"

// Learning material used @ learnopengl.com

// Directions the Camera can be moved
enum CAMERA_DIRECTION {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera {
private:

	// Frustum of the camera
	/*Frustum *frustum;*/

	// Camera Variables
	glm::vec3 position;
	glm::vec3 front, up, right;
	glm::vec3 worldUp;

	// Eular Angles
	float yaw;
	float pitch;

	// Camera Attributes
	float movementSpeed;
	float mouseSensitivity;
	float FOV;

	// Utility Variables
	float lastX = SCREEN_WDITH / 2.0f;
	float lastY = SCREEN_HEIGHT / 2.0f;
	bool firstMouse = true;

	float near_plane;
	float far_plane;
public:
	// Default Constructor using set default values
	Camera();

	// Constructor to set up initial position, yaw, and pitch with other variable set to default
	Camera(glm::vec3 _pos, float _yaw, float _pitch);

	// Constructor to set up all adjustable camera values
	Camera(glm::vec3 _pos, float _yaw, float _pitch, float _speed, float _sensitivity, float _zoom);

	~Camera() {};

	/*Frustum *GetFrustum();*/

	// Returns the current view matrix of the camera
	glm::mat4 GetViewMatrix();

	// Returns the position of the camera in world space
	glm::vec3 GetPosition();

	glm::vec3 GetFront();

	// Returns the up vector of the camera
	glm::vec3 GetUp();

	glm::vec3 GetRight();

	// Returns the FOV of the camera
	float GetFOV();

	// Returns the near and far values for the projection matrix
	float GetNear();
	float GetFar();

	// Functions that process the movement of the camera using input
	void ProcessKeyboard(CAMERA_DIRECTION _dir, float deltaTime);
	void ProcessMouseMovement(float eventX, float eventY);
	void ProcessMouseScroll(float offsetY);

	// Updates the stored vectors of the camera based on movement
	void UpdateCameraVectors();
	
	// Utility functions that set the rotation of the camera
	void SetRotationX(float _pitch);
	void SetRotationY(float _yaw);

	void SetPosition(glm::vec3 _pos);
};
#endif