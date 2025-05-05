#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 atexCoord;

uniform mat4 projectionM;
uniform mat4 viewM;
uniform mat4 modelM;

out vec3 normals;
out vec2 texCoord;
out vec3 fragPos;

void main()
{
    
    texCoord = atexCoord;
    normals = mat3( transpose ( inverse(modelM))) * aNormal;
    fragPos = vec3(modelM * vec4 (aPos, 1.0f));


    gl_Position = projectionM * viewM * modelM * vec4 (aPos, 1.0f);
    
}
