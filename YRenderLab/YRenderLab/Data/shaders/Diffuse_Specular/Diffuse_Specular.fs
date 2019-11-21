#version 330 core

//----------------- input and output
out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

// ----------- const variable--------------//
const float INV_PI = 0.31830988618;


//------------- struct -----------------
struct BSDF_DiffuseSpecular {
	vec3 colorFactor;
    sampler2D albedoTexture;// 0
	sampler2D normalTexture; // 1
	sampler2D specularTexture;// 2
};

struct DirectionalLight{
	vec3 L;         // 12   0
	vec3 dir;       // 12   16
	//mat4 ProjView;  // 64   32
};


// -----------------uniform block-----------
layout (std140) uniform DirectionalLights{
	DirectionalLight directionaLight;
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



// uniform BSDF_DiffuseSpecular bsdf;


// vec3 lambert_diffuse(vec3 albedo){
// 	return albedo * INV_PI;
// }



// void main(){
// 	texture(bsdf.albedoTexture,fs_in.FragPos);


// 	vec3 wi = -normalize(directionaLight.dir);
// 	vec3 wo = normalize(viewPos - fs_in.FragPos);
// 	vec3 wh = normalize(wi + wo);
// 	float cosTheta = max(dot(fs_in.Normal,wi),0);
// 	vec3 result = Blinn_Phong_BRDF(wi,wo) * directionaLight.L * cosTheta;
// 	//vec3 ambient = vec3(0.1f,0.1f,0.1f);
// 	FragColor = vec4(result,1.f); //gamma矫正
// }