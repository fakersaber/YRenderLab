#version 430 core

#include "../BRDF/StandardBRDF.h"

out vec3 FragColor;
in vec2 TexCoords;


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


//f the member is a structure, the base alignment of the structure is N
//where N is the largest base alignment value of any of its members, and rounded up to the base alignment of a vec4.
layout (std140) uniform DirectionalLights{
	int numDirectionalLight;// 16
	DirectionalLight directionaLights[MAX_DIRECTIONAL_LIGHTS];
};



uniform sampler2D GBuffer0;
uniform sampler2D GBuffer1;
uniform sampler2D GBuffer2;
//uniform sampler2D GBuffer3;

uniform sampler2D directionalLightDepthMap0; // 3
//uniform sampler2D directionalLightDepthMap1; // 4



//function declare
float DirectionalLightVisibility(vec3 normPos, float cosTheta, sampler2D depthMap);




void main(){
    vec4 data0 = texture(GBuffer0,TexCoords);
    vec4 data1 = texture(GBuffer1,TexCoords);
    vec4 data2 = texture(GBuffer2,TexCoords);

    vec3 FragPos = data0.xyz;
    float roughness = data0.w;
    vec3 normal = data1.xyz;
    float metallic = data1.w;
    vec3 albedo = data2.xyz;
    float ao = data2.w;

    vec3 result = vec3(0);

    vec3 wo = normalize(viewPos - FragPos);

    //point light


    //directional Light
	for(int i=0; i < numDirectionalLight; i++){
		vec3 wi = -normalize(directionaLights[i].dir);
		vec3 f = Standard_BRDF(normal, wo, wi, albedo, metallic, roughness);
		float cosTheta = max(dot(wi, normal), 0);
		//vec4 pos4 = directionaLights[i].ProjView * vec4(FragPos, 1);
		//注意ShadowMap都是压缩到了0~1,所以在NDC下的也要映射到0~1
		//vec3 normPos = ((pos4.xyz / pos4.w) + 1.0) / 2.0;

		//float visibility = DirectionalLightVisibility(normPos, cosTheta, directionalLightDepthMap0);

		result += cosTheta * f * directionaLights[i].L;
		//result += visibility * cosTheta * f * directionaLights[i].L;
	}

    FragColor = result;
}


float DirectionalLightVisibility(vec3 normPos, float cosTheta, sampler2D depthMap){
	float visibility = 0.0;
	vec2 texelSize = 1.0 / textureSize(depthMap, 0);
	//让自身的深度减bias或者采样出来的值加bias
	//不难推导，平面法线与Dir方向越大，所需要的bias越小，CosTheta刚好是normal与Dir的点乘，所以CosTheta越大Bias越小
	float bias = max(0.05 * (1.0 - cosTheta), 0.005);

	//3x3 pcf核
	for(int x = -1; x <= 1; ++x){
		for(int y = -1; y <= 1; ++y){
			float pcfDepth = texture(depthMap, normPos.xy + vec2(x, y) * texelSize).r; 
			//当 normPos.z - bias <= pcfDepth <=  normPos.z - 0.5 * bias时，  
			//则有 x = (pcfDepth - normPos.z + bias)/(0.5 * bias); 

			//normPos.z - bias > pcfDepth  全阴影

			// normPos.z - 0.5 * bias < pcfDepth 全光照

			//Visibility = 3x^2 - 2x^3;
			visibility += smoothstep(normPos.z - bias, normPos.z - 0.5 * bias, pcfDepth);      
		}
	}
	visibility /= 9.0;
	return visibility;
}