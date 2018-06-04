#include "Light.h"

Light::Light(glm::vec3 _colour, glm::vec3 _direction) {
	SetColour(_colour);
	direction = _direction;
	type = LIGHT_TYPE::DIRECTIONAL_LIGHT;
	shaderName = "defaultShadowDepth";
	//ShadowSetup();
}

Light::Light(glm::vec3 _colour, glm::vec3 _position, float _linear, float _quadratic) {
	SetColour(_colour);
	position = _position;
	linear = _linear;
	quadratic = _quadratic;
	type = LIGHT_TYPE::POINT_LIGHT;
	shaderName = "defaultShadowDepth";
	//ShadowSetup();
}

Light::Light(glm::vec3 _colour, glm::vec3 _position, glm::vec3 _direction, float _linear, float _quadratic, float _innerCutoff, float _outerCutoff) {
	SetColour(_colour);
	position = _position;
	direction = _direction;
	linear = _linear;
	quadratic = _quadratic;
	SetInnerCutoff(_innerCutoff);
	SetOuterCutoff(_outerCutoff);
	type = LIGHT_TYPE::SPOT_LIGHT;
	shaderName = "defaultShadowDepth";
	//ShadowSetup();
}

void Light::SetType(LIGHT_TYPE _type) {
	type = _type;
}

LIGHT_TYPE Light::GetType() {
	return type;
}

Shader* Light::GetShader() {
	return ShaderManager::GetInstance()->GetShader(shaderName);
}

void Light::SetColour(glm::vec3 _colour) {
	ambient = 0.3f * _colour;
	diffuse = _colour;
	specular = glm::clamp(2.0f * _colour, 0.0f, 1.0f);
}

void Light::SetAmbientColour(glm::vec3 _ambient) {
	ambient = _ambient;
}

void Light::SetDiffuseColour(glm::vec3 _diffuse) {
	diffuse = _diffuse;
}

void Light::SetSpecularColour(glm::vec3 _specular) {
	specular = _specular;
}

void Light::SetDirection(glm::vec3 _direction) {
	direction = _direction;
}

void Light::SetLinear(float _linear) {
	linear = _linear;
}

void Light::SetQuadratic(float _quadratic) {
	quadratic = _quadratic;
}

void Light::SetInnerCutoff(float _innerCutoff) {
	innerCutOff = glm::cos(glm::radians(_innerCutoff));
}

void Light::SetOuterCutoff(float _outerCutoff) {
	outerCutOff = glm::cos(glm::radians(_outerCutoff));
}

void Light::SetPosition(glm::vec3 _pos) {
	position = _pos;
}

glm::vec3 Light::GetColour() {
	return diffuse;
}

glm::vec3 Light::GetAmbientColour() {
	return ambient;
}

glm::vec3 Light::GetDiffuseColour() {
	return diffuse;
}

glm::vec3 Light::GetSpecularColour() {
	return specular;
}

glm::vec3 Light::GetDirection() {
	return direction;
}

glm::vec3 Light::GetPosition() {
	return position;
}

float Light::GetLinear() {
	return linear;
}

float Light::GetQuadratic() {
	return quadratic;
}

float Light::GetInnerCutoff() {
	return innerCutOff;
}

float Light::GetOuterCutoff() {
	return outerCutOff;
}

void Light::BindUniforms(Shader* _shader, int pointIndex, int spotIndex) {
	if (type == LIGHT_TYPE::DIRECTIONAL_LIGHT) {
		_shader->SetVec3("directionalLight.ambient", ambient);
		_shader->SetVec3("directionalLight.diffuse", diffuse);
		_shader->SetVec3("directionalLight.specular", specular);
		_shader->SetVec3("directionalLight.direction", direction);

		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(glm::vec3(direction * 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = lightProjection - lightView;

		_shader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

		_shader->SetInt("shadowMap", 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);
	}
	else if (type == LIGHT_TYPE::POINT_LIGHT) {
		_shader->SetVec3("pointLights[" + std::to_string(pointIndex) + "].ambient", ambient);
		_shader->SetVec3("pointLights[" + std::to_string(pointIndex) + "].diffuse", diffuse);
		_shader->SetVec3("pointLights[" + std::to_string(pointIndex) + "].specular", specular);
		_shader->SetVec3("pointLights[" + std::to_string(pointIndex) + "].position", position);
		_shader->SetFloat("pointLights[" + std::to_string(pointIndex) + "].linear", linear);
		_shader->SetFloat("pointLights[" + std::to_string(pointIndex) + "].quadratic", quadratic);
	}
	else if (type == LIGHT_TYPE::SPOT_LIGHT) {
		_shader->SetVec3("spotLights[" + std::to_string(spotIndex) + "].ambient", ambient);
		_shader->SetVec3("spotLights[" + std::to_string(spotIndex) + "].diffuse", diffuse);
		_shader->SetVec3("spotLights[" + std::to_string(spotIndex) + "].specular", specular);
		_shader->SetVec3("spotLights[" + std::to_string(spotIndex) + "].position", position);
		_shader->SetVec3("spotLights[" + std::to_string(spotIndex) + "].direction", direction);
		_shader->SetFloat("spotLights[" + std::to_string(spotIndex) + "].linear", linear);
		_shader->SetFloat("spotLights[" + std::to_string(spotIndex) + "].quadratic", quadratic);
		_shader->SetFloat("spotLights[" + std::to_string(spotIndex) + "].innerCutOff", innerCutOff);
		_shader->SetFloat("spotLights[" + std::to_string(spotIndex) + "].outerCutOff", outerCutOff);
	}
}

void Light::BindSpaceMatrix() {
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(glm::vec3(direction * 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	Shader* shader = ShaderManager::GetInstance()->GetShader(shaderName);
	shader->Use();
	shader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, shadowWidth, shadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}

glm::mat4 Light::GetLightSpaceMatrix() {
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(glm::vec3(direction * 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	return lightProjection * lightView;
}

void Light::ShadowSetup() {
	if (type == DIRECTIONAL_LIGHT) {
		glGenFramebuffers(1, &depthMapFBO);
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void Light::CalculateShadows(glm::mat4 _model) {

}