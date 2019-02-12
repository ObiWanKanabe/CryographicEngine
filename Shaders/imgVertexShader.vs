#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 normal;

// Defining how the max amount of shadow maps we can have in our cascade
#define NR_CASCADES 4
#define NR_SPOTLIGHTS 4

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out vec4 FragPosLightSpace[NR_CASCADES];

uniform mat4 model;
uniform mat4 normalMatrix;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix[NR_CASCADES];

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoords = aTexCoord;
	FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(normalMatrix) * normal;
    for (int i = 0; i < NR_CASCADES; i++) {
    	FragPosLightSpace[i] = lightSpaceMatrix[i] * vec4(FragPos, 1.0);
    }
}