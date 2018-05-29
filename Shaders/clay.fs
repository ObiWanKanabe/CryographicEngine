#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in mat3 TBN;

// The Material of the fragment
struct Material {
sampler2D diffuse1;
sampler2D normal1;
};

// Three Light types available
struct DirectionalLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
};

struct PointLight {
float linear;
float quadratic;
vec3 ambient;
vec3 diffuse;
vec3 specular;
vec3 position;
};

struct SpotLight {
float linear;
float quadratic;
float innerCutOff;
float outerCutOff;
vec3 ambient;
vec3 diffuse;
vec3 specular;
vec3 position;
vec3 direction;
};

// We define how many point lights and spot lights we'll allow in the world
#define NR_POINT_LIGHTS 4
#define NR_SPOT_LIGHTS 4

uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform vec3 cameraPos;
uniform samplerCube skybox;

// Functions 
vec3 DirLightResult(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 PointLightResult(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 SpotLightResult(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{

// All calculations of lighting will be added to this
vec3 result = vec3(0,0,0);

// Getting the normal from our bump map
vec3 norm = texture(material.normal1, TexCoords).rgb;
// Converting from its range of [0, 1] to [-1, 1]
norm = normalize(norm * 2.0 - 1.0);
norm = normalize(TBN * norm);

vec3 viewDir = normalize(cameraPos - FragPos);

// The reflection of the normal along the view direction
vec3 refl = reflect(-viewDir, norm);

// Calculate the one directional light allowed in the world
result += DirLightResult(directionalLight, norm, viewDir);

// Calculate all the point lights in the world
for (int i = 0; i < NR_POINT_LIGHTS; i++) {
result += PointLightResult(pointLights[i], norm, FragPos, viewDir); 
}

// Calculate all the spot lights in the world
for (int i = 0; i < NR_SPOT_LIGHTS; i++) {
result += SpotLightResult(spotLights[i], norm, FragPos, viewDir);
}

// The final result fragment colour
FragColor = vec4(result, 1.0);
}

// Directional Lights need the normal vector and view direction
vec3 DirLightResult(DirectionalLight light, vec3 normal, vec3 viewDir)
{
// The light direction vector only takes into account its own direction
vec3 lightDir = normalize(-light.direction);

// We're using Blinn-Phong here and calculating an imaginary halfway vector
// between the view direction and light direction
// This will be used for the specular light
vec3 halfwayDir = normalize(lightDir + viewDir);

// Diffuse shading
float diff = max(dot(normal, lightDir), 0.0);

// Combine the results
vec3 ambient  = light.ambient  * vec3(texture(material.diffuse1, TexCoords)) * 0.5;
vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse1, TexCoords));

return (ambient + diffuse);
}

vec3 PointLightResult(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
// The light direction vector is facing the fragment
vec3 lightDir = normalize(light.position - fragPos);

// We're using Blinn-Phong here and calculating an imaginary halfway vector
// between the view direction and light direction
// This will be used for the specular light
vec3 halfwayDir = normalize(lightDir + viewDir);

// Diffuse shading
float diff = max(dot(normal, lightDir), 0.0);

// Attenuation
float distance    = length(light.position - fragPos);
float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance));    

// Combine the results
vec3 ambient  = light.ambient  * vec3(texture(material.diffuse1, TexCoords)) * 0.5;
vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse1, TexCoords));
ambient  *= attenuation;
diffuse  *= attenuation;

return (ambient + diffuse);
} 

vec3 SpotLightResult(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
// The light direction vector is facing the fragment
vec3 lightDir = normalize(light.position - fragPos);

// We're using Blinn-Phong here and calculating an imaginary halfway vector
// between the view direction and light direction
// This will be used for the specular light
vec3 halfwayDir = normalize(lightDir + viewDir);

// Diffuse shading
float diff = max(dot(normal, lightDir), 0.0);

// Attenuation
float distance    = length(light.position - FragPos);
float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance));  

// Spotlight Effect
float theta = dot(lightDir, normalize(-light.direction)); 
float epsilon = (light.innerCutOff - light.outerCutOff);
float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

// Combine the results
vec3 ambient = light.ambient * vec3(texture(material.diffuse1, TexCoords)) * 0.5;
vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse1, TexCoords));
diffuse  *= intensity;
ambient  *= attenuation; 
diffuse   *= attenuation;

return (ambient + diffuse);
} 