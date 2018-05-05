#ifndef LIGHT_H
#define LIGHT_H

#include "ShaderManager.h"
#include "Vertex.h"

enum LIGHT_TYPE {
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT
};

class Light {
public:

	// A Directional light has a colour and direction
	Light(glm::vec3 _colour, glm::vec3 _direction);

	// A Point light has a colour, position, and attenuation values
	Light(glm::vec3 _colour, glm::vec3 _position, float _linear, float _quadratic);

	// A Spot Light has a colour, position, direction, attenuation values and inner and outer cutoff
	Light(glm::vec3 _colour, glm::vec3 _position, glm::vec3 _direction, float _linear, float _quadratic, float _innerCutoff, float _outerCutoff);


	// NOTE: If you're changing the light all type specific values need to be set
	// Failing to do so will yield unwanted results, such as lights being at origin, or simply not working
	void SetType(LIGHT_TYPE _type);
	LIGHT_TYPE GetType();

	// Utility functions
	void SetColour(glm::vec3 _colour);
	void SetAmbientColour(glm::vec3 _ambient);
	void SetDiffuseColour(glm::vec3 _diffuse);
	void SetSpecularColour(glm::vec3 _specular);
	void SetDirection(glm::vec3 _direction);
	void SetPosition(glm::vec3 _posiition);
	void SetLinear(float _linear);
	void SetQuadratic(float _quadratic);
	void SetInnerCutoff(float _innerCutoff);
	void SetOuterCutoff(float _outerCutoff);
	
	const glm::vec3 GetColour();
	const glm::vec3 GetAmbientColour();
	const glm::vec3 GetDiffuseColour();
	const glm::vec3 GetSpecularColour();
	const glm::vec3 GetDirection();
	const glm::vec3 GetPosition();
	const float GetLinear();
	const float GetQuadratic();
	const float GetInnerCutoff();
	const float GetOuterCutoff();
	

	// Binding the uniforms of the shader
	virtual void BindUniforms(Shader* _shader, int pointIndex, int spotIndex);

private:

	// The type of light
	LIGHT_TYPE type;

	// Ambient colour of the light
	glm::vec3 ambient;

	// Main diffuse colour of the light
	glm::vec3 diffuse;

	// Shine colour of the light
	glm::vec3 specular;

	// Direction of the directional light and spotlight
	// Measured in a unit vector
	glm::vec3 direction;

	// Position of the light in world space
	glm::vec3 position;

	// Constant of the point light to calculate attenuation
	// Is set to 1.0 by default to ensure intensity acts normally
	float constant;

	// Linear of the point light to calculate attenuation
	// Reduces the intensity in a linear way
	float linear;

	// Quadratic of the point light to calculate attenuation
	// Reduces the intensity in a quadratric way
	float quadratic;

	// Inner Cutoff of the spotlight
	// Measured in degrees out of the position and through the direction
	float innerCutOff;

	// Outer Cutoff of the spotlight
	// Measured in degrees out of the position and through the direction
	float outerCutOff;

};

#endif
