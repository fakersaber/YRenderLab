#version 330 core

out vec4 FragColor;

in vec3 TexCoords;

layout (std140) uniform Environment{
    vec3 colorFactor;
    float intensity;
    bool haveSkyBox;
    bool haveEnvironment;
};

uniform samplerCube skybox;

void main(){
    vec3 ambient = texture(skybox, TexCoords).xyz;
	FragColor = vec4(ambient,1.f);
}
