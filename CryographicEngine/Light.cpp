#include "Light.h"

Light::Light(glm::vec3 _colour, glm::vec3 _direction) {
	SetColour(_colour);
	direction = _direction;
	type = LIGHT_TYPE::DIRECTIONAL_LIGHT;
	shaderName = "defaultShadowDepth";
	ShadowSetup();
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

		_shader->SetMat4("lightSpaceMatrix[0]", lightSpaceMatrix[0]);
		_shader->SetMat4("lightSpaceMatrix[1]", lightSpaceMatrix[1]);
		_shader->SetMat4("lightSpaceMatrix[2]", lightSpaceMatrix[2]);

		_shader->SetInt("shadowMap[0]", 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap[0]);
		_shader->SetInt("shadowMap[1]", 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, depthMap[1]);
		_shader->SetInt("shadowMap[2]", 4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, depthMap[2]);
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

glm::mat4 Light::GetLightSpaceMatrix(glm::vec3 _pos, glm::vec3 front, int index) {

	glm::vec3 pos = _pos + front * frustum_size[index]/2.0f;

	glm::mat4 lightProjection = glm::ortho(-frustum_size[index], frustum_size[index], -frustum_size[index], frustum_size[index], near_plane[index], far_plane[index]);
	glm::mat4 lightView = glm::lookAt(direction * - frustum_size[index] * 4.0f + pos, pos, glm::vec3(0.0f, 1.0f, 0.0f));
	
	lightSpaceMatrix[index] = lightProjection * lightView;

	/*float height_near[3];
	float width_near[3];

	float height_far[3];
	float width_far[3];

	glm::vec3 center_near[3];
	glm::vec3 center_far[3];

	glm::vec3 near_top_left[3];
	glm::vec3 near_top_right[3];
	glm::vec3 near_bottom_left[3];
	glm::vec3 near_button_right[3];

	glm::vec3 far_top_left[3];
	glm::vec3 far_top_right[3];
	glm::vec3 far_bottom_left[3];
	glm::vec3 far_bottom_right[3];

	float ar = 1200.0f / 900.0f;
	float tanFOV = tanf(90.0f / 2.0f);

	for (int i = 0; i < 3; i++) {
		height_near[i] = 2.0f * tanFOV * frustum_end[i];
		width_near[i] = height_near[i] * ar;

		height_far[i] = 2 * tanFOV * frustum_end[i + 1];
		width_far[i] = height_far[i] * ar;

		center_near[i] = _pos + front * frustum_end[i];
		center_far[i] = _pos + front * frustum_end[i + 1];

		near_top_left[3] = center_near[i] + glm::vec3(0.0f, 1.0f, 0.0f) * height_near[i] / 2.0f - glm::vec3(1.0f, 0.0f, 0.0f) * width_near[i] / 2.0f;
		near_top_right[3] = center_near[i] + glm::vec3(0.0f, 1.0f, 0.0f) * height_near[i] / 2.0f + glm::vec3(1.0f, 0.0f, 0.0f) * width_near[i] / 2.0f;
		near_bottom_left[3] = center_near[i] - glm::vec3(0.0f, 1.0f, 0.0f) * height_near[i] / 2.0f - glm::vec3(1.0f, 0.0f, 0.0f) * width_near[i] / 2.0f;
		near_button_right[3] = center_near[i] - glm::vec3(0.0f, 1.0f, 0.0f) * height_near[i] / 2.0f + glm::vec3(1.0f, 0.0f, 0.0f) * width_near[i] / 2.0f;

		far_top_left[3] = center_far[i] + glm::vec3(0.0f, 1.0f, 0.0f) * height_far[i] / 2.0f - glm::vec3(1.0f, 0.0f, 0.0f) * width_far[i] / 2.0f;
		far_top_right[3] = center_far[i] + glm::vec3(0.0f, 1.0f, 0.0f) * height_far[i] / 2.0f + glm::vec3(1.0f, 0.0f, 0.0f) * width_far[i] / 2.0f;
		far_bottom_left[3] = center_far[i] - glm::vec3(0.0f, 1.0f, 0.0f) * height_far[i] / 2.0f - glm::vec3(1.0f, 0.0f, 0.0f) * width_far[i] / 2.0f;
		far_bottom_right[3] = center_far[i] - glm::vec3(0.0f, 1.0f, 0.0f) * height_far[i] / 2.0f + glm::vec3(1.0f, 0.0f, 0.0f) * width_far[i] / 2.0f;
	}*/

	return lightSpaceMatrix[index];
}

void Light::ShadowSetup() {

	// Closest Shadow Map
	glGenFramebuffers(3, &depthMapFBO[0]);
	glGenTextures(3, &depthMap[0]);
	glBindTexture(GL_TEXTURE_2D, depthMap[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[0]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap[0], 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	near_plane[0] = 1.0f;
	far_plane[0] = 50.0f;
	frustum_size[0] = 5.0f;

	// Middle Shadow Map
	glBindTexture(GL_TEXTURE_2D, depthMap[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[1]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap[1], 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	near_plane[1] = 1.0f;
	far_plane[1] = 200.0f;
	frustum_size[1] = 20.0f;

	// Far Shadow Map

	glBindTexture(GL_TEXTURE_2D, depthMap[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[2]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap[2], 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	near_plane[2] = 1.0f;
	far_plane[2] = 500.0f;
	frustum_size[2] = 50.0f;

}

void Light::PrepareShadow(int index) {
	glViewport(0, 0, shadowWidth, shadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[index]);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClear(GL_DEPTH_BUFFER_BIT);
}

GLuint Light::GetShadowMap() {
	return depthMap[1];
}