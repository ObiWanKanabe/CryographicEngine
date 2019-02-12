#include "PointLight.h"

PointLight::PointLight(glm::vec3 _colour, glm::vec3 _position, float _linear, float _quadratic) {
	SetColour(_colour);
	position = _position;
	linear = _linear;
	quadratic = _quadratic;
	ShadowSetup();
}

void PointLight::BindUniforms(Shader* _shader, int pointIndex, int spotIndex) {
	_shader->SetVec3("pointLights[" + std::to_string(pointIndex) + "].ambient", ambient);
	_shader->SetVec3("pointLights[" + std::to_string(pointIndex) + "].diffuse", diffuse);
	_shader->SetVec3("pointLights[" + std::to_string(pointIndex) + "].specular", specular);
	_shader->SetVec3("pointLights[" + std::to_string(pointIndex) + "].position", position);
	_shader->SetFloat("pointLights[" + std::to_string(pointIndex) + "].linear", linear);
	_shader->SetFloat("pointLights[" + std::to_string(pointIndex) + "].quadratic", quadratic);
	pointIndex++;
}

glm::mat4 PointLight::GetLightSpaceMatrix(Camera* camera, Window* window, int index) {
	// In Progress

	// Creating our lightSpace matrix based on the frustum size given and the position
	glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), static_cast<float>(window->GetWidth()/window->GetHeight()), 2.0f, 50.0f);
	glm::mat4 lightView = glm::lookAt(position, position, glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix[index] = lightProjection * lightView;

	return lightSpaceMatrix[index];
}

void PointLight::ShadowSetup() {

	updateShadows = false; // for now

	near_plane = 1.0f;
	far_plane = 25.0f;

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// Creating and binding our six Shadow Maps
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);
	
	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowSize, shadowSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void PointLight::PrepareShadow(int index) {
	glViewport(0, 0, shadowSize, shadowSize);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClear(GL_DEPTH_BUFFER_BIT);
}

bool PointLight::HasShadows() {
	if (diffuse.r <= 0.2f && diffuse.g <= 0.2f && diffuse.b <= 0.2f)
		return false;

	if (!updateShadows)
		return false;

	return true;
}

void PointLight::SetColour(glm::vec3 _colour) {
	ambient = 0.3f * _colour;
	diffuse = _colour;
	specular = glm::clamp(2.0f * _colour, 0.0f, 1.0f);
}

void PointLight::SetAmbientColour(glm::vec3 _ambient) {
	ambient = _ambient;
}

void PointLight::SetDiffuseColour(glm::vec3 _diffuse) {
	diffuse = _diffuse;
}

void PointLight::SetSpecularColour(glm::vec3 _specular) {
	specular = _specular;
}

void PointLight::SetPosition(glm::vec3 _pos) {
	position = _pos;
}

void PointLight::SetLinear(float _linear) {
	linear = _linear;
}

void PointLight::SetQuadratic(float _quadratic) {
	quadratic = _quadratic;
}

glm::vec3 PointLight::GetColour() {
	return diffuse;
}

glm::vec3 PointLight::GetAmbientColour() {
	return ambient;
}

glm::vec3 PointLight::GetDiffuseColour() {
	return diffuse;
}

glm::vec3 PointLight::GetSpecularColour() {
	return specular;
}

glm::vec3 PointLight::GetPosition() {
	return position;
}

float PointLight::GetLinear() {
	return linear;
}

float PointLight::GetQuadratic() {
	return quadratic;
}

int PointLight::GetNumDepthMaps() {
	return 6;
}