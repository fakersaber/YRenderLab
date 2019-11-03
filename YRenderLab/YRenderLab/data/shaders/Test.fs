#version 330 core

#include "Lights/DirectLight.h"

in vec3 Normal;
in vec3 WorldPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 _CameraPos;


void main()
{
    FragColor = vec4(1.f,1.f,1.f,1.f);
}