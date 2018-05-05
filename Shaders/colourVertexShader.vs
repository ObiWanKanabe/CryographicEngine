#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colour;
layout (location = 2) in vec3 normal;

out vec3 ourColour;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 normalMatrix;
uniform mat4 view;
uniform mat4 projection;

void main()
{
gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0);
ourColour = colour;
FragPos = vec3(model * vec4(position, 1.0));
Normal = mat3(normalMatrix) * normal;
};