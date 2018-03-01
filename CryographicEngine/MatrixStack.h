#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtc/type_ptr.hpp>
#include <stack>


class MatrixStack {
private:
	std::stack<glm::mat4> modelMatrixStack;
	std::stack<glm::mat4> projectionMatrixStack;
	bool modelMatrixStackDirty = true;
	bool projectionMatrixStackDirty = true;
public:
	MatrixStack();
	~MatrixStack();

	void Orthographic(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
	void Perspective(float fovY, float aspect, float nearZ, float farZ);
	void LookAt(glm::vec3 pos, glm::vec3 at, glm::vec3 up);
	void LookAt(float camX, float camY, float camZ, float atX, float atY, float atZ, float upX, float upY, float upZ);
	glm::mat4 GetModelMatrix();
	glm::mat4 GetProjectionMatrix();
	const float* GetModelMatrixValue();
	const float* GetProjectionMatrixValue();
	void SetModelMatrixToIdentity();
	void SetProjectionMatrixToIdentity();
	void PushModelMatrix();
	void PopModelMatrix();
	void PushProjectionMatrix();
	void PopProjectionMatrix();
	void Rotate(const glm::quat rotation);
	void Rotate(float angle, float x, float y, float z);
	void Scale(glm::vec3 scale);
	void Scale(float x, float y, float z);
	void Translate(glm::vec3 translation);
	void Translate(float x, float y, float z);
	bool IsModelMatrixDirty();
	bool IsProjectionMatrixDirty();
	void ClearModelMatrixDirty();
	void ClearProjectionMatrixDirty();
};

#endif
