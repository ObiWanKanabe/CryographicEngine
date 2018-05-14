#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;  

out vec2 TexCoords;
out vec3 FragPos;
out mat3 TBN;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
// Calculating TBN matrix for normal mapping
// This uses the tangents, bitangents and normals in their local space
// and results in a matrix to transform them back to world space
vec3 T = vec3(0, 0, 0);
T = normalize(vec3(model * vec4(aTangent,   0.0)));
vec3 N = vec3(0, 0, 0);
N = normalize(vec3(model * vec4(aNormal,    0.0)));

T = normalize(T - dot(T, N) * N);

vec3 B = vec3(0, 0, 0);
B = cross(N,T);

if (dot(cross(N, T), B) < 0.0)
    T = T * -1.0;

// Finally transposing the orthogonal matrix to get its inverse
TBN = mat3(T,B,N);

TexCoords = aTexCoords;
FragPos = vec3(model * vec4(aPos, 1.0));
FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
gl_Position = projection * view * model * vec4(aPos, 1.0);
}