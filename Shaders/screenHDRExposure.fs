#version 330 core
layout (location = 0) out vec4 FragColour;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform float gamma;
uniform float exposure;

void main()
{ 
  
    vec3 hdrColour = texture(screenTexture, TexCoords).rgb;
    
    // Tone mapping based on our exposure value
    vec3 mapped = vec3(1.0) - exp(-hdrColour * exposure);
    
    // Using gamma correction to base our final scene on
    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColour = vec4(mapped, 1.0);
}