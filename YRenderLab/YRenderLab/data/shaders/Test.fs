#version 330 core

//#include "Lights/DirectLight.h"
struct AmbientLight{
    vec3 lightColor;
    float intensity;
};



struct DirectLight{
    vec3 direction;
    vec3 lightColor;
};




in vec3 Normal;
in vec3 WorldPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform AmbientLight _AmbientLight;
uniform DirectLight _DirectLight;
uniform vec3 _CameraPos;


void main()
{
    vec3 ambient = _AmbientLight.lightColor * _AmbientLight.intensity;
    vec3 diffuse = max(dot(normalize(Normal),_DirectLight.direction),0.f) * _DirectLight.lightColor;

    FragColor = vec4(ambient + diffuse,1.f);
}