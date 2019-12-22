#version 330 core

#define PI            3.14159265359
#define TWO_PI        6.28318530718

out vec4 FragColor;

in vec3 WorldPos;


uniform samplerCube environmentMap;
uniform float roughness;
uniform float resolution;


vec3 SchlickGGX_Sample(vec2 Xi, vec3 N, float roughness){
    float a = roughness * roughness;
    float phi = TWO_PI * Xi.x;
    float cosTheta = sqrt((1.f - Xi.y)/(1.f + (a*a - 1.f)*Xi.y));
    float sinTheta = sqrt(1.f - cosTheta * cosTheta);

    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    //暂时使用线性变换,注意H的生成空间up轴为z轴，所以up取(0,0,1)
    vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);
	
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}



void main(){
    float SolidAngleTexel = 4.f * PI / (6.f * resolution * resolution);
    vec3 N = normalize(WorldPos);
    //想根据D(H)采样L,先采样H
}