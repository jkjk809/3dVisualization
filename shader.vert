#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aColor;

uniform mat4 model; // Model matrix (transforms object space to world space)
uniform mat4 view;  // View matrix (transforms world space to camera space)
uniform mat4 projection; // Projection matrix (transforms camera space to NDC)
uniform vec3 myColor; 
out vec3 color;

void main()
{
    // Combine transformations: projection * view * model * position
    gl_Position = projection * view * model * vec4(position, 1.0);
    color = vec3(model * vec4(position, 1.0)); 
}