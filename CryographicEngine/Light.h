#ifndef LIGHT_H
#define LIGHT_H

#include "ShaderManager.h"
#include "Camera.h"
#include <stdio.h>
#include <limits>
#include <math.h>

class Light {
public:

	Light() {};

	// Binding the uniforms of the shader
	virtual void BindUniforms(Shader* _shader, int pointIndex, int spotIndex) = 0;

	// Returns the light space matrix to be used when rendering objects to the lights depth map
	virtual glm::mat4 GetLightSpaceMatrix(Camera *camera, int index) = 0;

	// Setting up each lights shadow configuration on creation
	virtual void ShadowSetup() = 0;

	// Preparing the shadow map for rendering, including changing the viewport and binding the FBO
	virtual void PrepareShadow(int index) = 0;

	// This will opt out of the depth rendering based on early factors to increase performance
	virtual bool HasShadows() = 0;

	// Utility functions used by all light types
	virtual void SetColour(glm::vec3 _colour) = 0;
	virtual void SetAmbientColour(glm::vec3 _ambient) = 0;
	virtual void SetDiffuseColour(glm::vec3 _diffuse) = 0;
	virtual void SetSpecularColour(glm::vec3 _specular) = 0;
	virtual void SetPosition(glm::vec3 _posiition) = 0;
	
	virtual glm::vec3 GetColour() = 0;
	virtual glm::vec3 GetAmbientColour() = 0;
	virtual glm::vec3 GetDiffuseColour() = 0;
	virtual glm::vec3 GetSpecularColour() = 0;
	virtual glm::vec3 GetPosition() = 0;

	virtual int GetNumDepthMaps() = 0;

};

#endif
