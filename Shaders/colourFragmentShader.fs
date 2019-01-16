#version 330 core
layout (location = 0) out vec4 FragColour;
layout (location = 1) out vec4 BloomColour;

in vec3 ourColour;
in vec3 FragPos;
in vec3 Normal;

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

void main()
{

vec3 ambient = light.ambient * material.ambient;

vec3 norm = normalize(Normal);
vec3 lightDir = normalize(light.position - FragPos);

float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = light.diffuse * (diff * material.diffuse);

vec3 viewDir = normalize(cameraPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);

float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
vec3 specular = light.specular * (spec * material.specular);

vec3 result = ambient + diffuse + specular;
FragColour = vec4(result, 1.0f);

float bloom = dot(FragColour.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(bloom > 1.0)
        BloomColour = vec4(FragColour.rgb, 1.0);
    else
        BloomColour = vec4(0.0, 0.0, 0.0, 1.0);
};