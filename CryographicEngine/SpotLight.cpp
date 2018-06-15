#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 _colour, glm::vec3 _position, glm::vec3 _direction, float _linear, float _quadratic, float _innerCutoff, float _outerCutoff) {
	SetColour(_colour);
	position = _position;
	direction = _direction;
	linear = _linear;
	quadratic = _quadratic;
	SetInnerCutoff(_innerCutoff);
	SetOuterCutoff(_outerCutoff);
	ShadowSetup();
}

void SpotLight::BindUniforms(Shader* _shader, int pointIndex, int spotIndex) {
	_shader->SetVec3("spotLights[" + std::to_string(spotIndex) + "].ambient", ambient);
	_shader->SetVec3("spotLights[" + std::to_string(spotIndex) + "].diffuse", diffuse);
	_shader->SetVec3("spotLights[" + std::to_string(spotIndex) + "].specular", specular);
	_shader->SetVec3("spotLights[" + std::to_string(spotIndex) + "].position", position);
	_shader->SetVec3("spotLights[" + std::to_string(spotIndex) + "].direction", direction);
	_shader->SetFloat("spotLights[" + std::to_string(spotIndex) + "].linear", linear);
	_shader->SetFloat("spotLights[" + std::to_string(spotIndex) + "].quadratic", quadratic);
	_shader->SetFloat("spotLights[" + std::to_string(spotIndex) + "].innerCutOff", innerCutOff);
	_shader->SetFloat("spotLights[" + std::to_string(spotIndex) + "].outerCutOff", outerCutOff);
	spotIndex++;
}

glm::mat4 SpotLight::GetLightSpaceMatrix(Camera* camera, int index) {
	// Progress Ongoings

	return lightSpaceMatrix;
}

void SpotLight::ShadowSetup() {

	updateShadows = false; // for now

	near_plane = 1.0f;
	far_plane = 25.0f;

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// Creating and binding our Shadow Map
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);

	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowSize, shadowSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SpotLight::PrepareShadow(int index) {
	glViewport(0, 0, shadowSize, shadowSize);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClear(GL_DEPTH_BUFFER_BIT);
}

bool SpotLight::HasShadows() {
	if (diffuse.r <= 0.2f && diffuse.g <= 0.2f && diffuse.b <= 0.2f)
		return false;

	if (!updateShadows)
		return false;

	return true;
}

void SpotLight::SetColour(glm::vec3 _colour) {
	ambient = 0.3f * _colour;
	diffuse = _colour;
	specular = glm::clamp(2.0f * _colour, 0.0f, 1.0f);
}

void SpotLight::SetAmbientColour(glm::vec3 _ambient) {
	ambient = _ambient;
}

void SpotLight::SetDiffuseColour(glm::vec3 _diffuse) {
	diffuse = _diffuse;
}

void SpotLight::SetSpecularColour(glm::vec3 _specular) {
	specular = _specular;
}

void SpotLight::SetPosition(glm::vec3 _pos) {
	position = _pos;
}

void SpotLight::SetDirection(glm::vec3 _direction) {
	direction = _direction;
}

void SpotLight::SetLinear(float _linear) {
	linear = _linear;
}

void SpotLight::SetQuadratic(float _quadratic) {
	quadratic = _quadratic;
}

void SpotLight::SetInnerCutoff(float _innerCutoff) {
	innerCutOff = glm::cos(glm::radians(_innerCutoff));
}

void SpotLight::SetOuterCutoff(float _outerCutoff) {
	outerCutOff = glm::cos(glm::radians(_outerCutoff));
}

glm::vec3 SpotLight::GetColour() {
	return diffuse;
}

glm::vec3 SpotLight::GetAmbientColour() {
	return ambient;
}

glm::vec3 SpotLight::GetDiffuseColour() {
	return diffuse;
}

glm::vec3 SpotLight::GetSpecularColour() {
	return specular;
}

glm::vec3 SpotLight::GetPosition() {
	return position;
}

glm::vec3 SpotLight::GetDirection() {
	return direction;
}

float SpotLight::GetLinear() {
	return linear;
}

float SpotLight::GetQuadratic() {
	return quadratic;
}

float SpotLight::GetInnerCutoff() {
	return innerCutOff;
}

float SpotLight::GetOuterCutoff() {
	return outerCutOff;
}

int SpotLight::GetNumDepthMaps() {
	return 1;
}