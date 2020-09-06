#ifndef _YRENDER_BRDF_STANDARDBRDF_H_
#define _YRENDER_BRDF_STANDARDBRDF_H_

#include "FDG.h"

vec3 Standard_BRDF(vec3 normal, vec3 wo, vec3 wi, vec3 albedo, float metallic, float roughness) {
	vec3 wh = normalize(wo + wi);
	
	float D = Trowbridge_Reitz_GGX_D(normal, wh, roughness);
	float G = SchlickGGX_Smith_G(normal, wo, wi, roughness);
	vec3 F = Fr_epic(wi, wh, albedo, metallic);
	
	vec3 specular = D * G * F / (4.0f * dot(wh, wo) * dot(wh, wi));
	
	vec3 diffuse = albedo * INV_PI;
	
	vec3 kS = 1 - F;
	vec3 kD = (1-metallic) * (1 - kS);
	
	vec3 rst = kD * diffuse + specular;

	return rst;
}


#endif