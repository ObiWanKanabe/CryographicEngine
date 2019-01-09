#version 330 core
out vec4 FragColour;

in vec2 TexCoords;
in vec3 FragPos;
in mat3 TBN;
in vec4 FragPosLightSpace;

// The Material of the fragment
struct Material {
float shininess;
float reflectiveness;
sampler2D diffuse1;
sampler2D specular1;
sampler2D normal1;
sampler2D reflective1;
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
uniform sampler2D shadowMap;

// Functions 
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec4 fragPosLightSpace);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace);

void main()
{

// All calculations of lighting will be added to this
vec3 result;;

// Reflection result will be added as well
vec3 reflect_result;

// Getting the normal from our bump map
vec3 norm = texture(material.normal1, TexCoords).rgb;
// Converting from its range of [0, 1] to [-1, 1]
norm = normalize(norm * 2.0 - 1.0);
norm = normalize(TBN * norm);

vec3 viewDir = normalize(cameraPos - FragPos);

// The reflection of the normal along the view direction
vec3 refl = reflect(-viewDir, norm);

// Calculate the one directional light allowed in the world
result += CalcDirLight(directionalLight, norm, viewDir, FragPosLightSpace);

// Calculate all the point lights in the world
for (int i = 0; i < NR_POINT_LIGHTS; i++) {
result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, FragPosLightSpace); 
}

// Calculate all the spot lights in the world
for (int i = 0; i < NR_SPOT_LIGHTS; i++) {
result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir, FragPosLightSpace);
}

// Calculating the reflection intensity
float reflect_intensity = texture(material.reflective1, TexCoords).r;

// If reflection map is present, reflect the skybox texture
if(reflect_intensity > 0.2)
    reflect_result = material.reflectiveness * vec3(texture(skybox, refl)) * reflect_intensity;

// The final result fragment colour
FragColour = vec4(result, 1.0) + vec4(reflect_result, 1.0);
}

// Calculate shadows for the directional light
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {

// Convert from our orthographic projection space to perspective and transform to [0, 1] range to match our texture
vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
projCoords = projCoords * 0.5 + 0.5;

// This will be our closest depth value to compare our current fragments depth value to
float closestDepth = texture(shadowMap, projCoords.xy).r;  

// The depth value of the current fragment from the light's point of view
float currentDepth = projCoords.z;   

// Giving a small bias to prevent shadow acne and darkening surrounding fragments
// This is dependent on the angle between the normal and light's direction
float bias = max(0.001 * (1.0 - dot(normal, lightDir)), 0.0005);

// Using PCF to sample numerous times from the shadow depth map
// Sampling the surrounding texels to produce a smoother shadow result
// This is done 9 times in the example and then we take the average
float shadow = 0.0;
vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
for(int x = -1; x <= 1; ++x) {
    for(int y = -1; y <= 1; ++y) {
        float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
        shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
    }    
}
shadow /= 9.0;

// Checking to see if the projected coordinates are outside of the orthographic frustum
// Don't apply shadows if it is
if (projCoords.z > 1.0)
	shadow = 0.0;

return shadow;
}

// Directional Lights need the normal vector and view direction
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec4 fragPosLightSpace)
{
// The light direction vector only takes into account its own direction
vec3 lightDir = normalize(-light.direction);

// We're using Blinn-Phong here and calculating an imaginary halfway vector
// between the view direction and light direction
// This will be used for the specular light
vec3 halfwayDir = normalize(lightDir + viewDir);

// Diffuse shading
float diff = max(dot(normal, lightDir), 0.0);

// Specular shading
float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

// Combine the results
vec3 ambient  = light.ambient  * vec3(texture(material.diffuse1, TexCoords)) * 0.5;
vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse1, TexCoords));
vec3 specular = light.specular * spec * vec3(texture(material.specular1, TexCoords));

// Apply shadow calculations
float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);       
vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular));

return result;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace)
{
// The light direction vector is facing the fragment
vec3 lightDir = normalize(light.position - fragPos);

// We're using Blinn-Phong here and calculating an imaginary halfway vector
// between the view direction and light direction
// This will be used for the specular light
vec3 halfwayDir = normalize(lightDir + viewDir);

// Diffuse shading
float diff = max(dot(normal, lightDir), 0.0);

// Specular shading
float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

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

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace)
{
// The light direction vector is facing the fragment
vec3 lightDir = normalize(light.position - fragPos);

// We're using Blinn-Phong here and calculating an imaginary halfway vector
// between the view direction and light direction
// This will be used for the specular light
vec3 halfwayDir = normalize(lightDir + viewDir);

// Diffuse shading
float diff = max(dot(normal, lightDir), 0.0);

// Specular shading
float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

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