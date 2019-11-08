#version 330 core
layout (location = 0) in vec3 iModelPos;
layout (location = 1) in vec3 iModelNormal;
layout (location = 2) in vec2 iTexCoords;

out vec3 Normal;
out vec3 WorldPos;
out vec2 TexCoords;

uniform mat4 _WorldToCamera;
uniform mat4 _CameraToClip;


void main()
{
    WorldPos = iModelPos;
    Normal = iModelNormal;
    TexCoords = iTexCoords;
    gl_Position = _CameraToClip * _WorldToCamera * vec4(WorldPos,1.0f);

}
