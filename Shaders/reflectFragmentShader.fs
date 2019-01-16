#version 330 core
layout (location = 0) out vec4 FragColour;
layout (location = 1) out vec4 BloomColour;

in vec3 Normal;
in vec3 Position;

struct Material {
vec3 ambient;
vec3 diffuse;
vec3 specular;
float shininess;
};

struct Light {
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{          

vec3 I = normalize(Position - cameraPos);
vec3 R = reflect(I, normalize(Normal)); 

vec3 ambient = vec3(texture(skybox, R));

vec3 norm = normalize(Normal);
vec3 lightDir = normalize(light.position - Position);

float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = vec3(texture(skybox, R));

vec3 viewDir = normalize(cameraPos - Position);
vec3 reflectDir = reflect(-lightDir, norm);

float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
vec3 specular = light.specular * spec * vec3(texture(skybox, R));

vec3 result = specular;
  

    FragColour = vec4(result ,1.0f) + vec4(texture(skybox, R).rgb, 1.0);
}