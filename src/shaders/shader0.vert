#version 330 core

layout (location = 0) in vec3 aPos;


uniform mat4 viewM;
uniform mat4 projectionM;

void main()
{
    gl_Position = projectionM * viewM * vec4 (aPos, 1.0f);
}