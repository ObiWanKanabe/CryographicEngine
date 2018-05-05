#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

// The Material of the fragment
struct Material {
sampler2D diffuse1;
sampler2D specular1;
sampler2D normal1;
sampler2D reflective1;
float shininess;
};

// Three Light types available
struct DirectionalLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
};

struct PointLight {
vec3 ambient;
vec3 diffuse;
vec3 specular;
vec3 position;
float linear;
float quadratic;
};

struct SpotLight {
vec3 ambient;
vec3 diffuse;
vec3 specular;
vec3 position;
vec3 direction;
float linear;
float quadratic;
float innerCutOff;
float outerCutOff;
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
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{

// All calculations of lighting will be added to this
vec3 result = vec3(0.0);

// Reflection result will be added as well
vec3 reflect_result;

// The normal and view direction on the fragment, being used for lighting calculations
vec3 norm = normalize(Normal);
vec3 viewDir = normalize(cameraPos - FragPos);

// The reflection of the normal along the view direction
vec3 refl = reflect(viewDir, norm);

// Calculate the one directional light allowed in the world
result += CalcDirLight(directionalLight, norm, viewDir);

// Calculate all the point lights in the world
for (int i = 0; i < NR_POINT_LIGHTS; i++) {
result += CalcPointLight(pointLights[i], norm, FragPos, viewDir); 
}

// Calculate all the spot lights in the world
for (int i = 0; i < NR_SPOT_LIGHTS; i++) {
result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
}

// Calculating the reflection intensity
float reflect_intensity = texture(material.reflective1, TexCoords).r;

// If reflection map is present, reflect the skybox texture
if(reflect_intensity > 0.1)
    reflect_result = vec3(texture(skybox, refl)) * reflect_intensity;

// The final result fragment colour
FragColor = vec4(result, 1.0f) + vec4(reflect_result, 1.0f);
}

// Directional Lights need the normal vector and view direction
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
// The light direction vector only takes into account its own direction
vec3 lightDir = normalize(-light.direction);

// Diffuse shading
float diff = max(dot(normal, lightDir), 0.0);

// Specular shading
vec3 reflectDir = reflect(-lightDir, normal);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

// Combine the results
vec3 ambient  = light.ambient  * vec3(texture(material.diffuse1, TexCoords)) * 0.5;
vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse1, TexCoords));
vec3 specular = light.specular * spec * vec3(texture(material.specular1, TexCoords));

return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
// The light direction vector is facing the fragment
vec3 lightDir = normalize(light.position - fragPos);

// Diffuse shading
float diff = max(dot(normal, lightDir), 0.0);

// Specular shading
vec3 reflectDir = reflect(-lightDir, normal);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

// Attenuation
float distance    = length(light.position - fragPos);
float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance));    

// Combine the results
vec3 ambient  = light.ambient  * vec3(texture(material.diffuse1, TexCoords)) * 0.5;
vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse1, TexCoords));
vec3 specular = light.specular * spec * vec3(texture(material.specular1, TexCoords));
ambient  *= attenuation;
diffuse  *= attenuation;
specular *= attenuation;

return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
// The light direction vector is facing the fragment
vec3 lightDir = normalize(light.position - fragPos);

// Diffuse shading
float diff = max(dot(normal, lightDir), 0.0);

// Specular shading
vec3 reflectDir = reflect(-lightDir, normal); 
float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

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
vec3 specular = light.specular * spec * vec3(texture(material.specular1, TexCoords));  
diffuse  *= intensity;
specular *= intensity;
ambient  *= attenuation; 
diffuse   *= attenuation;
specular *= attenuation;   
        
return (ambient + diffuse + specular);
} 