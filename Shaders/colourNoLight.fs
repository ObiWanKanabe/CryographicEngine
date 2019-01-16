#version 330 core
layout (location = 0) out vec4 FragColour;
layout (location = 1) out vec4 BloomColour;

in vec3 ourColour;

void main()
{
FragColour = vec4(ourColour, 1.0f);

// Extracting bloom colour output to be blurred
float bloom = dot(FragColour.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(bloom > 1.0)
        BloomColour = vec4(FragColour.rgb, 1.0);
    else
        BloomColour = vec4(0.0, 0.0, 0.0, 1.0);
};