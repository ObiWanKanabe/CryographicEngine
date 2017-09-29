#version 330 core
out vec4 Fragcolor;

in vec3 ourColour;

void main()
{
Fragcolor = vec4(ourColour, 1.0f);
};