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
struct BSDF_Diffuse {
	vec3 colorFactor;
	bool haveAlbedoTexture;
    sampler2D albedoTexture;// 0
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


uniform BSDF_Diffuse bsdf;

void main(){
    // vec3 albedo = bsdf.colorFactor * texture(bsdf.albedoTexture, fs_in.TexCoords).xyz;
    // vec3 diffuse = albedo * INV_PI; //lambert brdf,diffuse中各个方向相同所以跟wi无关
    // vec3 wi = -normalize(directionaLight.dir);
    // float cosTheta = max(dot(fs_in.Normal,wi),0);
    // vec3 result = cosTheta * diffuse * directionaLight.L;
    // gamma 校正
	FragColor = texture(bsdf.albedoTexture, fs_in.TexCoords);
	//FragColor = vec4(bsdf.colorFactor,1.f);
}