#version 330 core
out vec4 FragColor;
in vec3 color;

uniform vec3 colorStart;
uniform vec3 colorEnd;
//uniform sampler2D ourTexture;
void main()
{

  float gradientFactor = (color.y + 1.0f) / 7.0; 

    vec3 interpolatedColor = mix(colorStart, colorEnd, gradientFactor);

    FragColor = vec4(interpolatedColor, 0.5);
//FragColor = vec4(color, 1.0);
};