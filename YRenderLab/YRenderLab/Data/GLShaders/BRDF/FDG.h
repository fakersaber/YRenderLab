#ifndef _YRENDER_SHADER_BRDF_FDG_H_    
#define _YRENDER_SHADER_BRDF_FDG_H_

#include "../Math/basic.h"

#define DEFAULT_REFLECTANCE 0.04

//--------------D function------------------//
float Trowbridge_Reitz_GGX_D(vec3 norm, vec3 h, float roughness);


//--------------G function------------------//
float SchlickGGX_Smith_G(vec3 N, vec3 wo, vec3 wi, float roughness);
float SchlickGGX_G1(vec3 norm, vec3 w, float roughness);


//--------------F function------------------//
vec3 Fr_epic(vec3 wi, vec3 h, vec3 albedo, float metallic);
vec3 MetalFr_Approx(vec3 wo, vec3 norm, vec3 F0, float roughness);
vec3 MetalGetF0(vec3 albedo, float metallic);

//---------------------------------------//
float Trowbridge_Reitz_GGX_D(vec3 norm, vec3 h, float roughness){
	float NoH = max(dot(norm, h),0.0);
	float alpha = roughness * roughness;
	float alpha2 = alpha * alpha;
	float cos2Theta = NoH * NoH;
	float t = (alpha2 - 1) * cos2Theta + 1;
	return alpha2 / (PI * t * t);
}

float SchlickGGX_G1(vec3 norm, vec3 w, float roughness) {
    // remapping roughness using (Roughness+1)/2  --- epic paper
	float k = (roughness+1) * (roughness+1) / 8;
	float Ndotw = max(0.f, dot(norm, w));
	return Ndotw / (Ndotw * (1 - k) + k);
}

float SchlickGGX_Smith_G(vec3 N, vec3 wo, vec3 wi, float roughness){
    return SchlickGGX_G1(N, wo, roughness) * SchlickGGX_G1(N, wi, roughness);
}

vec3 Fr_epic(vec3 wi, vec3 h, vec3 albedo, float metallic) {
	vec3 F0 = MetalGetF0(albedo, metallic);
	float HdotL = dot(h, wi);
	return F0 + exp2((-5.55473f * HdotL - 6.98316f) * HdotL) * (vec3(1.0f) - F0);
}

vec3 MetalFr_Approx(vec3 wo, vec3 norm, vec3 F0, float roughness){
    float cosTheta = max(dot(wo, norm), 0);
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 MetalGetF0(vec3 albedo, float metallic) {
    return mix(vec3(DEFAULT_REFLECTANCE), albedo, metallic);
}

#endif