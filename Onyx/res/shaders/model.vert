


#version 330 core

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;

out vec3 frag_normal;
out vec3 frag_pos;

void main()
{
    gl_Position = (projectionMatrix * modelViewMatrix) * vec4(in_position, 1.0f);
    frag_pos = vec3(modelViewMatrix * vec4(in_position, 1.0f));
    frag_normal = mat3(transpose(inverse(modelViewMatrix))) * in_normal;  
} 