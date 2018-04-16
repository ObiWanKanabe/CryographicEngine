#include "MatrixStack.h"

MatrixStack::MatrixStack() {

	glm::mat4 model(1.0f);
	glm::mat4 projection(1.0f);
	modelMatrixStack.push(model);
	projectionMatrixStack.push(projection);
}

MatrixStack::~MatrixStack() {

}

void MatrixStack::Orthographic(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
	projectionMatrixStack.top() = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
	projectionMatrixStackDirty = true;
}

void MatrixStack::Perspective(float fovY, float aspect, float nearZ, float farZ) {
	projectionMatrixStack.top() = glm::perspective(fovY, aspect, nearZ, farZ);
	projectionMatrixStackDirty = true;
}

void MatrixStack::LookAt(const glm::vec3 pos, const glm::vec3 at, const glm::vec3 up) {
	modelMatrixStack.top() = glm::lookAt(pos, at, up);
	modelMatrixStackDirty = true;
}

void MatrixStack::LookAt(float camX, float camY, float camZ, float atX, float atY, float atZ, float upX, float upY, float upZ)
{
	LookAt(glm::vec3(camX, camY, camZ), glm::vec3(atX, atY, atZ), glm::vec3(upX, upY, upZ));
	modelMatrixStackDirty = true;
}

glm::mat4 MatrixStack::GetModelMatrix() {
	return modelMatrixStack.top();
}

glm::mat4 MatrixStack::GetProjectionMatrix() {
	return projectionMatrixStack.top();
}

const float* MatrixStack::GetModelMatrixValue() {
	return glm::value_ptr(GetModelMatrix());
}

const float* MatrixStack::GetProjectionMatrixValue() {
	return glm::value_ptr(GetProjectionMatrix());
}

void MatrixStack::SetModelMatrixToIdentity() {
	modelMatrixStack.top() = glm::mat4(1.0f);
	modelMatrixStackDirty = true;
}

void MatrixStack::SetProjectionMatrixToIdentity() {
	projectionMatrixStack.top() = glm::mat4(1.0f);
	projectionMatrixStackDirty = true;
}
void MatrixStack::PushModelMatrix() {
	modelMatrixStack.push(modelMatrixStack.top());
}

void MatrixStack::PopModelMatrix() {
	modelMatrixStack.pop();
	modelMatrixStackDirty = true;
}

void MatrixStack::PushProjectionMatrix() {
	projectionMatrixStack.push(projectionMatrixStack.top());
}

void MatrixStack::PopProjectionMatrix() {
	projectionMatrixStack.pop();
	projectionMatrixStackDirty = true;
}

void MatrixStack::Rotate(glm::quat q) {
	float angle = glm::angle(q);
	glm::vec3 vec = glm::axis(q);

	modelMatrixStack.top() = modelMatrixStack.top() * glm::rotate(angle, vec);
	modelMatrixStackDirty = true;
}

void MatrixStack::Rotate(float angle, float x, float y, float z) {

	modelMatrixStack.top() = modelMatrixStack.top() * glm::rotate(angle, glm::vec3(x, y, z));
	modelMatrixStackDirty = true;
}

void MatrixStack::Scale(glm::vec3 scale) {
	modelMatrixStack.top() = modelMatrixStack.top() * glm::scale(scale);
	modelMatrixStackDirty = true;
}

void MatrixStack::Scale(float x, float y, float z) {
	modelMatrixStack.top() = modelMatrixStack.top() * glm::scale(glm::vec3(x, y, z));
	modelMatrixStackDirty = true;
}

void MatrixStack::Translate(glm::vec3 translation) {
	modelMatrixStack.top() = modelMatrixStack.top() * glm::translate(translation);
	modelMatrixStackDirty = true;
}

void MatrixStack::Translate(float x, float y, float z) {
	modelMatrixStack.top() = modelMatrixStack.top() * glm::translate(glm::vec3(x, y, z));
	modelMatrixStackDirty = true;
}

bool MatrixStack::IsModelMatrixDirty() {
	return modelMatrixStackDirty;
}

bool MatrixStack::IsProjectionMatrixDirty() {
	return projectionMatrixStackDirty;
}

void MatrixStack::ClearModelMatrixDirty() {
	modelMatrixStackDirty = false;
}

void MatrixStack::ClearProjectionMatrixDirty() {
	projectionMatrixStackDirty = false;
}
