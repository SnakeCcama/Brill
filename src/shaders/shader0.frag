#version 330 core

out vec4 fragColor;

uniform vec3 polColor;


void main()
{
    fragColor = vec4(polColor, 1.0f);
}


