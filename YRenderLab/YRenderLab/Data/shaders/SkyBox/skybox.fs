#version 330 core

out vec4 FragColor;

in vec3 TexCoords;

layout (std140) uniform Environment{
    vec3 colorFactor;
    float intensity;
    bool haveSkyBox;
    bool haveEnvironment;
}


void main()
{    
    vec3 ambient = texture(skybox, texcoord).rgb;
	FragColor = vec4(ambient, 1);
}
