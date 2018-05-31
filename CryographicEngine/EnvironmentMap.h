#ifndef ENVIRONMENTMAP_H
#define ENVIRONMENTMAP_H

#include "CubeMapCamera.h"

class EnvironmentMap {
public:
	EnvironmentMap(int _size, glm::vec3 _position);

	void SetUp(int _size, glm::vec3 _position);

	void PreRender();
	void RenderToFace(int _index);
	void PostRender();

	void BindTexture();

	CubeMapCamera* GetCamera();

private:

	CubeMapCamera *camera;
	GLuint textureID;
	GLuint FBO;

	int size;
};

#endif
