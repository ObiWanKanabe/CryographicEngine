#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform float gamma;

void main()
{ 
  
    vec3 hdrColour = texture(screenTexture, TexCoords).rgb;
    
    // Tone mapping using reinhard technique
    vec3 mapped = hdrColour / (hdrColour + vec3(1.0f));
    
    // Using gamma correction to base our final scene on
    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0);
}