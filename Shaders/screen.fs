#version 330 core
out vec4 FragColour;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
    FragColour = texture(screenTexture, TexCoords);

    // Calculator if this fragment will have bloom applied
    float brightness = dot(FragColour.rgb, vec3(0.2126, 0.7152, 0.0722));

    // If above threshold, output the bloom colour as the fragment colour
    // Otherwise, ignore the bloom colour and set to 0
    if (brightness > 1.0) 
        BloomColour = vec4(FragColour.rgb, 1.0);
    else
        BloomColour = vec4(0.0, 0.0, 0.0, 1.0);
}