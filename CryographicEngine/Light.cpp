#include "Light.h"

Light::Light(glm::vec3 _colour, glm::vec3 _direction) {
	SetColour(_colour);
	direction = _direction;
	type = LIGHT_TYPE::DIRECTIONAL_LIGHT;
	shaderName = "defaultShadowDepth";
	DirectionalCascadeShadowSetup();
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

void Light::SetPosition(glm::vec3 _pos) {
	position = _pos;
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

void Light::SetFrustumSizes(float _near, float _middleNear, float _middleFar, float _far) {
	frustum_size[0] = _near;
	far_plane[0] = _near * 20.0f;

	frustum_size[1] = _middleNear;
	far_plane[1] = _middleNear * 20.0f;

	frustum_size[2] = _middleFar;
	far_plane[2] = _middleFar * 20.0f;

	frustum_size[3] = _far;
	far_plane[3] = _far * 20.0f;
}

void Light::SetNumberOfShadowCascades(int _cascades) {
	numCascades = _cascades;
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
		_shader->SetMat4("lightSpaceMatrix[3]", lightSpaceMatrix[3]);

		for (int i = 0; i < 4; i++) {
			_shader->SetInt("shadowMap[" + std::to_string(i) + "]", i + 2);
		}

		for (int i = 0; i < numCascades; i++) {
			glActiveTexture(GL_TEXTURE2 + i);
			glBindTexture(GL_TEXTURE_2D, depthMap[i]);
		}
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

glm::mat4 Light::GetLightSpaceMatrix(Camera* camera, int index) {


	// Proper Spliting of the the view frustum in progress

	//int NUM_CASCADES = 3;
	//const int NUM_CORNERS = 8;
	//cascade_end[0] = camera->GetNear();
	//cascade_end[1] = camera->GetFar() * 0.075f;
	//cascade_end[2] = camera->GetFar() * 0.2f;
	//cascade_end[3] = camera->GetFar();

	//glm::mat4 cameraView = camera->GetViewMatrix();
	//glm::mat4 cameraInverse = glm::inverse(cameraView);

	//glm::vec3 pos = camera->GetPosition() + camera->GetFront() * frustum_size[index] / 2.0f;

	//float worldTexelSize = frustum_size[index] / (float)1024;
	//pos = worldTexelSize * camera->GetPosition() / worldTexelSize;

	//glm::mat4 lightView = glm::lookAt(direction, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//float aspect = 900.0f / 1200.0f;
	//float tanHalfHFOV = tanf(glm::radians(camera->GetFOV() / 2.0f));
	//float tanHalfVFOV = tanf(glm::radians((camera->GetFOV() * aspect) / 2.0f));

	//	float xn = cascade_end[index] * tanHalfHFOV;
	//	float xf = cascade_end[index + 1] * tanHalfHFOV;
	//	float yn = cascade_end[index] * tanHalfVFOV;
	//	float yf = cascade_end[index + 1] * tanHalfVFOV;

	//	glm::vec4 frustumCorners[NUM_CORNERS] = {

	//		glm::vec4(xn, yn, cascade_end[index], 1.0),
	//		glm::vec4(-xn, yn, cascade_end[index], 1.0),
	//		glm::vec4(xn, -yn, cascade_end[index], 1.0),
	//		glm::vec4(-xn, -yn, cascade_end[index], 1.0),

	//		glm::vec4(xf, yf, cascade_end[index + 1], 1.0),
	//		glm::vec4(-xf, yf, cascade_end[index + 1], 1.0),
	//		glm::vec4(xf, -yf, cascade_end[index + 1], 1.0),
	//		glm::vec4(-xf, -yf, cascade_end[index + 1], 1.0)
	//	};

	//	glm::vec4 frustumCornersLight[NUM_CORNERS];

	//	float minX = std::numeric_limits<float>::max();
	//	float maxX = std::numeric_limits<float>::min();
	//	float minY = std::numeric_limits<float>::max();
	//	float maxY = std::numeric_limits<float>::min();
	//	float minZ = std::numeric_limits<float>::max();
	//	float maxZ = std::numeric_limits<float>::min();

	//	for (int j = 0; j < NUM_CORNERS; j++) {

	//		// Transform the frustum coordinate from view to world space
	//		glm::vec4 vW = cameraInverse * frustumCorners[j];

	//		// Transform the frustum coordinate from world to light space
	//		frustumCornersLight[j] = lightView * vW;

	//		minX = glm::min(minX, frustumCornersLight[j].x);
	//		maxX = glm::max(maxX, frustumCornersLight[j].x);
	//		minY = glm::min(minY, frustumCornersLight[j].y);
	//		maxY = glm::max(maxY, frustumCornersLight[j].y);
	//		minZ = glm::min(minZ, frustumCornersLight[j].z);
	//		maxZ = glm::max(maxZ, frustumCornersLight[j].z);
	//	}

	//	glm::mat4 lightProjection = glm::mat4();

	//	lightProjection[0][0] = 2.0f / (maxX - minX);
	//	lightProjection[1][1] = 2.0f / (maxY - minY);
	//	lightProjection[2][2] = -2.0f / (maxZ - minZ);
	//	lightProjection[3][0] = -((maxX + minX) / (maxX - minX));
	//	lightProjection[3][1] = -((maxY + minY) / (maxY - minY));
	//	lightProjection[3][2] = -((maxZ + minZ) / (maxZ - minZ));
	//	lightProjection[3][3] = 1.0f;

	//	glm::mat4 projection = glm::perspective(camera->GetFOV(), aspect, camera->GetNear(), camera->GetFar());
	//	glm::vec4 view(0.0f, 0.0f, cascade_end[index + 1], 1.0f);
	//	glm::vec4 clip = projection * view;
	//	cascadeEndClipSpace[index] = clip.z;

	// Working version - First Implementation

	// Setting the position of the orthographic position from the camera's facing position
	glm::vec3 pos = camera->GetPosition() + camera->GetFront() * frustum_size[index]/2.0f;

	// Making sure that we move the view frustum based on texel sized movements
	// Rotating the camera does not cause any "shimmering" effect
	float worldTexelSize = frustum_size[index] / (float)1024;
	pos = worldTexelSize * camera->GetPosition() / worldTexelSize;

	// Creating our lightSpace matrix based on the frustum size given and the position of the camera
	glm::mat4 lightProjection = glm::ortho(-frustum_size[index], frustum_size[index], -frustum_size[index], frustum_size[index], near_plane[index], far_plane[index]);
	glm::mat4 lightView = glm::lookAt(direction * -frustum_size[index] * 10.0f + pos, pos, glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix[index] = lightProjection * lightView;

	return lightSpaceMatrix[index];
}

void Light::DirectionalCascadeShadowSetup() {

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

	for (int i = 0; i < numCascades; i++) {
		glBindTexture(GL_TEXTURE_2D, depthMap[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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