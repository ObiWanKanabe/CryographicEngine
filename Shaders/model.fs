#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 Position;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_reflective1;
uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{    
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    float reflect_intensity = texture(texture_reflective1, TexCoords).r;
    vec4 reflect_colour;
    if(reflect_intensity > 0.1)
    	reflect_colour = texture(skybox, R) * reflect_intensity;
    FragColor = texture(texture_diffuse1, TexCoords) + 0.1 * texture(texture_specular1, TexCoords) + reflect_colour;
}