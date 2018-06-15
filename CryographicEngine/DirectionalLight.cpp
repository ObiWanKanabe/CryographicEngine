#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 _colour, glm::vec3 _direction) {
	SetColour(_colour);
	direction = _direction;
	ShadowSetup();
}

void DirectionalLight::BindUniforms(Shader* _shader, int pointIndex, int spotIndex) {
	_shader->SetVec3("directionalLight.ambient", ambient);
	_shader->SetVec3("directionalLight.diffuse", diffuse);
	_shader->SetVec3("directionalLight.specular", specular);
	_shader->SetVec3("directionalLight.direction", direction);

	_shader->SetMat4("lightSpaceMatrix[0]", lightSpaceMatrix[0]);
	_shader->SetMat4("lightSpaceMatrix[1]", lightSpaceMatrix[1]);
	_shader->SetMat4("lightSpaceMatrix[2]", lightSpaceMatrix[2]);
	_shader->SetMat4("lightSpaceMatrix[3]", lightSpaceMatrix[3]);

	for (int i = 0; i < 4; i++) {
		_shader->SetInt("shadowMap[" + std::to_string(i) + "]", i + 2);
	}

	for (int i = 0; i < numCascades; i++) {
		glActiveTexture(GL_TEXTURE2 + i);
		glBindTexture(GL_TEXTURE_2D, depthMap[i]);
	}
}

glm::mat4 DirectionalLight::GetLightSpaceMatrix(Camera* camera, int index) {
	// Working version - First Implementation

	// Setting the position of the orthographic position from the camera's facing position
	glm::vec3 pos = camera->GetPosition() + camera->GetFront() * frustum_size[index] / 2.0f * (glm::radians(180.0f - camera->GetFOV()));

	// Creating our lightSpace matrix based on the frustum size given and the position of the camera
	glm::mat4 lightProjection = glm::ortho(-frustum_size[index], frustum_size[index], -frustum_size[index], frustum_size[index], near_plane[index], far_plane[index]);
	glm::mat4 lightView = glm::lookAt(direction * -frustum_size[index] * 10.0f + pos, pos, glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix[index] = lightProjection * lightView;

	return lightSpaceMatrix[index];
}

void DirectionalLight::ShadowSetup() {

	// By default we use four cascades and values I felt were a good balance of quality/distance
	numCascades = 4;

	near_plane[0] = 1.0f;
	far_plane[0] = 150.0f;
	frustum_size[0] = 7.5f;

	near_plane[1] = 1.0f;
	far_plane[1] = 400.0f;
	frustum_size[1] = 20.0f;

	near_plane[2] = 1.0f;
	far_plane[2] = 700.0f;
	frustum_size[2] = 35.0f;

	near_plane[3] = 1.0f;
	far_plane[3] = 1500.0f;
	frustum_size[3] = 75.0f;

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// Creating and binding our four Shadow Maps
	glGenFramebuffers(4, &depthMapFBO[0]);
	glGenTextures(4, &depthMap[0]);

	for (int i = 0; i < 4; i++) {
		glBindTexture(GL_TEXTURE_2D, depthMap[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowSize, shadowSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap[i], 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void DirectionalLight::PrepareShadow(int index) {
	glViewport(0, 0, shadowSize, shadowSize);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[index]);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClear(GL_DEPTH_BUFFER_BIT);
}

bool DirectionalLight::HasShadows() {
	if (diffuse.r <= 0.2f && diffuse.g <= 0.2f && diffuse.b <= 0.2f)
		return false;

	if (numCascades == 0)
		return false;

	return true;
}

void DirectionalLight::SetColour(glm::vec3 _colour) {
	ambient = 0.3f * _colour;
	diffuse = _colour;
	specular = glm::clamp(2.0f * _colour, 0.0f, 1.0f);
}

void DirectionalLight::SetAmbientColour(glm::vec3 _ambient) {
	ambient = _ambient;
}

void DirectionalLight::SetDiffuseColour(glm::vec3 _diffuse) {
	diffuse = _diffuse;
}

void DirectionalLight::SetSpecularColour(glm::vec3 _specular) {
	specular = _specular;
}

void DirectionalLight::SetPosition(glm::vec3 _pos) {
	
}

void DirectionalLight::SetDirection(glm::vec3 _direction) {
	direction = _direction;
}

glm::vec3 DirectionalLight::GetColour() {
	return diffuse;
}

glm::vec3 DirectionalLight::GetAmbientColour() {
	return ambient;
}

glm::vec3 DirectionalLight::GetDiffuseColour() {
	return diffuse;
}

glm::vec3 DirectionalLight::GetSpecularColour() {
	return specular;
}

glm::vec3 DirectionalLight::GetPosition() {
	return glm::vec3(0.0f);
}

glm::vec3 DirectionalLight::GetDirection() {
	return direction;
}

int DirectionalLight::GetNumDepthMaps() {
	return numCascades;
}