#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace[4];

// The Material of the fragment
struct Material {
sampler2D diffuse1;
sampler2D specular1;
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

// Defining how the max amount of shadow maps we can have in our cascade
#define NR_CASCADES 4

uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform vec3 cameraPos;
uniform sampler2D shadowMap[NR_CASCADES];

// Functions 
float ShadowCalculation(vec4 fragPosLightSpace[NR_CASCADES], vec3 normal, vec3 lightDir);
int ShadowDebug(vec4 fragPosLightSpace[NR_CASCADES]);
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec4 fragPosLightSpace[NR_CASCADES]);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace[NR_CASCADES]);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace[NR_CASCADES]);

void main()
{

// All calculations of lighting will be added to this
vec3 result = vec3(0.0);

// The normal and view direction on the fragment, being used for lighting calculations
vec3 norm = normalize(Normal);
vec3 viewDir = normalize(cameraPos - FragPos);

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

// The final result fragment colour
FragColor = vec4(result, 1.0f);
}

// Calculate shadows for the directional light
float ShadowCalculation(vec4 fragPosLightSpace[NR_CASCADES], vec3 normal, vec3 lightDir) {

// Convert from our orthographic projection space to perspective and transform to [0, 1] range to match our texture
vec3 projCoords[NR_CASCADES];

for (int i = 0; i < NR_CASCADES; i++) {
	projCoords[i] = fragPosLightSpace[i].xyz / fragPosLightSpace[i].w;
	projCoords[i] = projCoords[i] * 0.5 + 0.5;
}

// The closest depth value to the camera in the shadowMap
float closestDepth[NR_CASCADES];
for (int i = 0; i < NR_CASCADES; i++) {
	closestDepth[i] = texture(shadowMap[i], projCoords[i].xy).r;   
}

// The depth value of the current fragment from the light's point of view
float currentDepth[NR_CASCADES];
for (int i = 0; i < NR_CASCADES; i++) {
	currentDepth[i] = projCoords[i].z;   
}

// Giving a small bias to prevent shadow acne and darkening surrounding fragments
// This is dependent on the angle between the normal and light's direction
float bias = max(0.001 * (1.0 - dot(normal, lightDir)), 0.0005);

int s = 0;
float shadow;

// We are roughly checking which depth is being taken for use in the shadow map
// This is only an estimation in this iteration, merely to show the concept of CSM
if (currentDepth[1] + 0.025 <= closestDepth[0] && texture(shadowMap[1], projCoords[1].xy).r > 0.1) {
	s = 1;
	if (currentDepth[2] + 0.05 <= closestDepth[1] && texture(shadowMap[2], projCoords[2].xy).r > 0.1) {
		s = 2;
		if (currentDepth[3] + 0.1 <= closestDepth[2] && texture(shadowMap[3], projCoords[3].xy).r > 0.1) {
			s = 3;
		} 
	}
}

// Using PCF to sample numerous times from the shadow depth map
// Sampling the surrounding texels to produce a smoother shadow result
// This is done 9 times in the example and then we take the average
vec2 texelSize = 1.0 / textureSize(shadowMap[s], 0);
for(int x = -1; x <= 1; ++x) {
    for(int y = -1; y <= 1; ++y) {
        float pcfDepth = texture(shadowMap[s], projCoords[s].xy + vec2(x, y) * texelSize).r; 
        shadow += currentDepth[s] - bias > pcfDepth  ? 1.0 : 0.0;        
    }    
}
shadow /= 9.0;


// Checking to see if the projected coordinates are outside of the orthographic frustum
// Don't apply shadows if it is
	if(projCoords[s].z > 1.0)
        shadow = 0.0;

return shadow;
}

// Quick Debug of CSM
int ShadowDebug(vec4 fragPosLightSpace[4]) {
// Convert from our orthographic projection space to perspective and transform to [0, 1] range to match our texture
vec3 projCoords[4];
for (int i = 0; i < 4; i++) {
	projCoords[i] = fragPosLightSpace[i].xyz / fragPosLightSpace[i].w;
	projCoords[i] = projCoords[i] * 0.5 + 0.5;
}

// The closest depth value to the camera in the shadowMap
float closestDepth[4];
for (int i = 0; i < 4; i++) {
	closestDepth[i] = texture(shadowMap[i], projCoords[i].xy).r;   
}

// The depth value of the current fragment from the light's point of view
float currentDepth[4];
for (int i = 0; i < 4; i++) {
	currentDepth[i] = projCoords[i].z;   
}

// Debugging to see which shadowMap we are taking the depth value
int s = 0;
if (currentDepth[1] + 0.025 <= closestDepth[0]) {
	s = 1;
	if (currentDepth[2] + 0.05 <= closestDepth[1]) {
		s = 2;
		if (currentDepth[3] + 0.1 <= closestDepth[2]) {
			s = 3;
		} 
	}
}
return s;
}

// Directional Lights need the normal vector and view direction
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec4 fragPosLightSpace[4])
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
float shadow = ShadowCalculation(fragPosLightSpace, normal, lightDir);       
vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular));

return result;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace[4])
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

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace[4])
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