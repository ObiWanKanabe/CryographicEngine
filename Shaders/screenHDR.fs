#version 330 core
layout (location = 0) out vec4 FragColour;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D bloomBlur;
uniform float gamma;

void main()
{ 
  
    vec3 hdrColour = texture(screenTexture, TexCoords).rgb;
    vec3 bloomColour = texture(bloomBlur, TexCoords).rgb;

    hdrColour += bloomColour; // additive blending
    
    // Tone mapping using reinhard technique
    vec3 mapped = hdrColour / (hdrColour + vec3(1.0));
    
    // Using gamma correction to base our final scene on
    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColour = vec4(mapped, 1.0);
}