#version 430 core

#include "../BRDF/StandardBRDF.h"

out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec3 Tangent;
} fs_in;


//-------------- material struct -------------//

struct BSDF_StandardPBR {
	vec3 colorFactor;
	//bool haveAlbedoTexture;
    sampler2D albedoTexture;

	float metallicFactor;
	//bool haveMetallicTexture;
    sampler2D metallicTexture;

	float roughnessFactor;
	//bool haveRoughnessTexture;
    sampler2D roughnessTexture;
	
	//bool haveAOTexture;
    sampler2D aoTexture;

	//bool haveNormalTexture;
	sampler2D normalTexture;
};

//----------- material end ---------------//


//----------- uniform struct -------------//

struct DirectionalLight{
	vec3 L;         // 12   0
	vec3 dir;       // 12   16
	//mat4 ProjView;  // 64   32
};

// 160
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



layout (std140) uniform DirectionalLights{
	int numDirectionalLight;// 16
	DirectionalLight directionaLights[MAX_DIRECTIONAL_LIGHTS];// 96 * MAX_DIRECTIONAL_LIGHTS = 96 * 8 = 768
};


uniform BSDF_StandardPBR bsdf;

uniform samplerCube irradianceMap; // 26
uniform samplerCube prefilterMap; // 27
uniform sampler2D   brdfLUT; // 28


//假定Tangent的坐标系是左手,所以在Cross时必须要tangent到normal
vec3 CalcBumpedNormal(vec3 normal, vec3 tangent, sampler2D normalTexture, vec2 texcoord){
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 bitangent = cross(tangent, normal);
    vec3 bumpMapNormal = texture(normalTexture, texcoord).xyz;
    bumpMapNormal = 2.0 * bumpMapNormal - 1.0;
    mat3 TBN = mat3(tangent, bitangent, normal);
    vec3 newNormal = TBN * bumpMapNormal;
    newNormal = normalize(newNormal);
    return newNormal;
}


void main(){
    vec3 albedo = texture(bsdf.albedoTexture,fs_in.TexCoords).xyz * bsdf.colorFactor;
    float metallic = texture(bsdf.metallicTexture,fs_in.TexCoords).x * bsdf.metallicFactor;
    float roughness = texture(bsdf.roughnessTexture,fs_in.TexCoords).x * bsdf.roughnessFactor;
    float ao = texture(bsdf.aoTexture,fs_in.TexCoords).x;

    vec3 normal = CalcBumpedNormal(normalize(fs_in.Normal), normalize(fs_in.Tangent), bsdf.normalTexture, fs_in.TexCoords);

    vec3 wo = normalize(viewPos - fs_in.FragPos);

    vec3 result = vec3(0);


	//directional light
	for(int i=0; i < numDirectionalLight; i++){
		vec3 wi = -normalize(directionaLights[i].dir);

		vec3 f = Standard_BRDF(normal, wo, wi, albedo, metallic, roughness);

		float cosTheta = max(dot(wi, normal), 0);
		
		// vec4 pos4 = directionaLights[i].ProjView * vec4(fs_in.FragPos, 1);
		// vec3 normPos = ((pos4.xyz / pos4.w) + 1) / 2;
		// float visibility = DirectionalLightVisibility(normPos, cosTheta, i);
		
		result +=  cosTheta * f * directionaLights[i].L;
	}


    // ambient light
	if(haveEnvironment) {
		vec3 F0 = MetalGetF0(albedo, metallic);
		vec3 F = MetalFr_Approx(wo, normal, F0, roughness);
		vec3 kS = F;
		vec3 kD = (1 - metallic) * (vec3(1) - kS);
		
		vec3 irradiance = haveSkybox ? texture(irradianceMap, normal).rgb : vec3(1);
		
		vec3 diffuse = irradiance * albedo;
		
		vec3 R = reflect(-wo, normal);
		const float MAX_REFLECTION_LOD = 4.0;
		// 用 R 来采样
		vec3 prefilteredColor = haveSkybox ? textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb : vec3(1);
		vec2 envBRDF = texture(brdfLUT, vec2(max(dot(normal, wo), 0.0), roughness)).rg;
		vec3 specular = prefilteredColor * (F0 * envBRDF.x + envBRDF.y);
		
		vec3 ambient = (kD * diffuse + specular) * ao * intensity * colorFactor;
		result += ambient;
	}
    
	//gamma 校正
	result = pow(result, vec3(1.0/2.2));

    FragColor = vec4(result,1.0);
    
}