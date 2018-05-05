#include "Light.h"

Light::Light(glm::vec3 _colour, glm::vec3 _direction) {
	SetColour(_colour);
	direction = _direction;
	type = LIGHT_TYPE::DIRECTIONAL_LIGHT;
}

Light::Light(glm::vec3 _colour, glm::vec3 _position, float _linear, float _quadratic) {
	SetColour(_colour);
	position = _position;
	linear = _linear;
	quadratic = _quadratic;
	type = LIGHT_TYPE::POINT_LIGHT;
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
}

void Light::SetType(LIGHT_TYPE _type) {
	type = _type;
}

LIGHT_TYPE Light::GetType() {
	return type;
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

const glm::vec3 Light::GetColour() {
	return diffuse;
}

const glm::vec3 Light::GetAmbientColour() {
	return ambient;
}

const glm::vec3 Light::GetDiffuseColour() {
	return diffuse;
}

const glm::vec3 Light::GetSpecularColour() {
	return specular;
}

const glm::vec3 Light::GetDirection() {
	return direction;
}

const glm::vec3 Light::GetPosition() {
	return position;
}

const float Light::GetLinear() {
	return linear;
}

const float Light::GetQuadratic() {
	return quadratic;
}

const float Light::GetInnerCutoff() {
	return innerCutOff;
}

const float Light::GetOuterCutoff() {
	return outerCutOff;
}

void Light::BindUniforms(Shader* _shader, int pointIndex, int spotIndex) {
	if (type == LIGHT_TYPE::DIRECTIONAL_LIGHT) {
		_shader->SetVec3("directionalLight.ambient", ambient);
		_shader->SetVec3("directionalLight.diffuse", diffuse);
		_shader->SetVec3("directionalLight.specular", specular);
		_shader->SetVec3("directionalLight.direction", direction);
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
		_shader->SetFloat("spotLights[" + std::to_string(spotIndex) + "].innerCutOff", innerCutOff);
		_shader->SetFloat("spotLights[" + std::to_string(spotIndex) + "].outerCutOff", outerCutOff);
	}
}