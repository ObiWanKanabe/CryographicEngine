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
vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));

// Checking to make sure the tangent is perpendicular
 T = normalize(T - dot(T, N) * N);

// Calculating the bitangent using the cross product
vec3 B = cross(N,T);

// Checking to make sure tangents aren't using a left handed system
if (dot(cross(N, T), B) < 0.0)
    T = T * -1.0;

// Finally creating the TBN matrix
TBN = mat3(T,B,N);

TexCoords = aTexCoords;
FragPos = vec3(model * vec4(aPos, 1.0));
FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
gl_Position = projection * view * model * vec4(aPos, 1.0);
}