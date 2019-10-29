#version 330 core
layout (location = 0) in vec3 worldPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;
uniform mat4 view;
uniform mat4 proj;
uniform vec4 pos;
void main()
{
    gl_Position = proj * view * vec4(worldPos,1.0f);
    ourColor = aColor;
}
