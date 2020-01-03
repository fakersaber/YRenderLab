#version 330 core

out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec3 Tangent;
} fs_in;



#define MAX_DIRECTIONAL_LIGHTS 8
const float PI = 3.14159265359;
const float INV_PI = 0.31830988618;


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
uniform samplerCube skybox; // 25
uniform samplerCube irradianceMap; // 26
uniform samplerCube prefilterMap; // 27
uniform sampler2D   brdfLUT; // 28


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

//------------------all func define ---------------------//
vec3 GetF0(vec3 albedo, float metallic) {
    return mix(vec3(0.04), albedo, metallic);
}

vec3 Fr_Approx(vec3 wo, vec3 norm, vec3 F0, float roughness){
    float cosTheta = max(dot(wo, norm), 0);
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

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
	vec3 F0 = GetF0(albedo, metallic);
	float HdotL = dot(h, wi);
	return F0 + exp2((-5.55473f * HdotL - 6.98316f) * HdotL) * (vec3(1.0f) - F0);
}

vec3 BRDF(vec3 norm, vec3 wo, vec3 wi, vec3 albedo, float metallic, float roughness) {
	vec3 wh = normalize(wo + wi);
	
	float D = Trowbridge_Reitz_GGX_D(norm, wh, roughness);
	float G = SchlickGGX_Smith_G(norm, wo, wi, roughness);
	vec3 F = Fr_epic(wi, wh, albedo, metallic);
	
	vec3 specular = D * G * F / (4.0f * dot(wh, wo) * dot(wh, wi));
	
	vec3 diffuse = albedo * INV_PI;
	
	vec3 kS = 1 - F;
	vec3 kD = (1-metallic) * (1 - kS);
	
	vec3 rst = kD * diffuse + specular;

	return rst;
}


void main(){
    vec3 albedo = texture(bsdf.albedoTexture,fs_in.TexCoords).xyz * bsdf.colorFactor;
    float metallic = texture(bsdf.metallicTexture,fs_in.TexCoords).x * bsdf.metallicFactor;
    float roughness = texture(bsdf.roughnessTexture,fs_in.TexCoords).x * bsdf.roughnessFactor;
    float ao = texture(bsdf.aoTexture,fs_in.TexCoords).x;

    vec3 normal = CalcBumpedNormal(normalize(fs_in.Normal), normalize(fs_in.Tangent), bsdf.normalTexture, fs_in.TexCoords);

    vec3 wo = normalize(viewPos - fs_in.FragPos);

    vec3 result = vec3(0);


	// directional light
	for(int i=0; i < numDirectionalLight; i++){
		vec3 wi = -normalize(directionaLights[i].dir);

		vec3 f = BRDF(normal, wo, wi, albedo, metallic, roughness);

		float cosTheta = max(dot(wi, normal), 0);
		
		// vec4 pos4 = directionaLights[i].ProjView * vec4(fs_in.FragPos, 1);
		// vec3 normPos = ((pos4.xyz / pos4.w) + 1) / 2;
		// float visibility = DirectionalLightVisibility(normPos, cosTheta, i);
		
		result +=  cosTheta * f * directionaLights[i].L;
	}


    // ambient light
	if(haveEnvironment) {
		vec3 F0 = GetF0(albedo, metallic);
		vec3 F = Fr_Approx(wo, normal, F0, roughness);
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