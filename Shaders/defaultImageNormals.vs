#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 normal;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 normalMatrix;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoords = aTexCoord;
	FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(normalMatrix) * normal;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}