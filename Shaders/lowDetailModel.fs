#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;

// The Material of the fragment
struct Material {
sampler2D diffuse1;
};

uniform Material material;

void main()
{

vec3 result = vec3(texture(material.diffuse1, TexCoords));

FragColor = vec4(result, 1.0f);
}