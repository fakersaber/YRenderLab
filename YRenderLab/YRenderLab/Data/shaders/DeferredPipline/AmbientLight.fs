#version 430 core

#include "../BRDF/StandardBRDF.h"

out vec3 FragColor;
in vec2 TexCoords;


layout (std140) uniform Camera{
	mat4 view;			// 64	0	
	mat4 projection;	// 64	64	
	vec3 viewPos;		// 12	128	
	float nearPlane;	// 4	140
	float farPlane;		// 4	144	
	float fov;			// 4	158	
	float ar;			// 4	152
};

layout (std140) uniform Environment{
	vec3 colorFactor;     // 12     0
	float intensity;      //  4    12
	bool haveSkybox;      //  4    16
	bool haveEnvironment; //  4    20
};

uniform sampler2D GBuffer0;
uniform sampler2D GBuffer1;
uniform sampler2D GBuffer2;
uniform sampler2D GBuffer3;

uniform samplerCube irradianceMap; 
uniform samplerCube prefilterMap; 
uniform sampler2D   brdfLUT; 

//除球协和quat处理irradianceMap外specular部分来自unreal
vec3 CalcAmbient(int ID, vec3 wo, vec3 norm, vec3 albedo, float roughness, float metallic, float ao);

vec3 CalcAmbient_Standard(vec3 wo, vec3 norm, vec3 albedo, float roughness, float metallic, float ao);

void main(){

    vec4 data0 = texture(GBuffer0,TexCoords);
    vec4 data1 = texture(GBuffer1,TexCoords);
    vec4 data2 = texture(GBuffer2,TexCoords);
	vec4 data3 = texture(GBuffer3,TexCoords);

    vec3 FragPos = data0.xyz;
    float roughness = data0.w;
    vec3 normal = data1.xyz;
    float metallic = data1.w;
    vec3 albedo = data2.xyz;
    float ao = data2.w;
	int ID = int(data3.w);


    vec3 wo = normalize(viewPos - FragPos);

    vec3 ambient = CalcAmbient(ID,wo,normal,albedo,roughness,metallic,ao);

    FragColor = ambient;
}


vec3 CalcAmbient(int ID, vec3 wo, vec3 norm, vec3 albedo, float roughness, float metallic, float ao){
	if(ID == 0){
		return CalcAmbient_Standard(wo, norm, albedo, roughness, metallic, ao);
	}else if(ID == 1){
		return vec3(0);
	}
}


vec3 CalcAmbient_Standard(vec3 wo, vec3 norm, vec3 albedo, float roughness, float metallic, float ao){
	vec3 F0 = MetalGetF0(albedo, metallic);
	vec3 F = MetalFr_Approx(wo, norm, F0, roughness);
	vec3 kS = F;
	vec3 kD = (1 - metallic) * (vec3(1) - kS);
		
	vec3 irradiance = haveSkybox ? texture(irradianceMap, norm).rgb : vec3(1);
		
	vec3 diffuse = irradiance * albedo;
		
	vec3 R = reflect(-wo, norm);
	const float MAX_REFLECTION_LOD = 4.0;
	// 用 R 来采样
	vec3 prefilteredColor = haveSkybox ? textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb : vec3(1);
	vec2 envBRDF = texture(brdfLUT, vec2(max(dot(norm, wo), 0.0), roughness)).rg;
	vec3 specular = prefilteredColor * (F0 * envBRDF.x + envBRDF.y);
		
	return (kD * diffuse + specular) * ao * intensity * colorFactor;
}