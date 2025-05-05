#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normals;
out vec3 fragPos;


uniform mat4 viewM;
uniform mat4 projectionM;
uniform mat4 modelM;


void main()
{
    
    fragPos = vec3(modelM * vec4(aPos, 1.0));
    normals = mat3(transpose(inverse(modelM))) * aNormal;


    gl_Position = projectionM * viewM * modelM * vec4 (aPos, 1.0f);
}