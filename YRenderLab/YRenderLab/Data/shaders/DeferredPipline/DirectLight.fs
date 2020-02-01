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



layout (std140) uniform DirectionalLights{
	int numDirectionalLight;// 16
	DirectionalLight directionaLights[MAX_DIRECTIONAL_LIGHTS];
};


uniform sampler2D GBuffer0;
uniform sampler2D GBuffer1;
uniform sampler2D GBuffer2;
//uniform sampler2D GBuffer3;


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


    //direction Light
	for(int i=0; i < numDirectionalLight; i++){
		vec3 wi = -normalize(directionaLights[i].dir);
		vec3 f = Standard_BRDF(normal, wo, wi, albedo, metallic, roughness);
		float cosTheta = max(dot(wi, normal), 0);

		// vec4 pos4 = directionaLights[i].ProjView * vec4(fs_in.FragPos, 1);
		// vec3 normPos = ((pos4.xyz / pos4.w) + 1) / 2;
		// float visibility = DirectionalLightVisibility(normPos, cosTheta, i);

		result +=  cosTheta * f * directionaLights[i].L;
	}

    FragColor = result;
}