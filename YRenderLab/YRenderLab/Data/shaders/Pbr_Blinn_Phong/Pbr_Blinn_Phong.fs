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
struct BSDF_blinnPhong {
	vec3 colorFactor;
	bool haveAlbedoTexture;
	float gloss;
    sampler2D albedoTexture;// 0
	sampler2D specularTexture;// 1
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



uniform BSDF_blinnPhong bsdf;



// ---------- function define
vec3 CalcBumpedNormal(vec3 normal, vec3 tangent, sampler2D normalTexture, vec2 texcoord){
	
}

vec3 lambert_diffuse(vec3 albedo){
	return albedo * INV_PI;
}

// Schlick Fresnel approximation
//SpecularColor其实更像是一种描述basecolor与reflect的关系，在金属工作流中插值到albedo，但在我这个不太物理的模型中就使用高光贴图
//其实可能直接用采样出来的高光颜色可能效果更好，看美术给这张贴图的意义在哪里，反正虚幻是0.08*高光贴图作为F0
vec3 F_Schlick(vec3 SpecularColor, float VoH){
	return SpecularColor + (vec3(1.f) - SpecularColor) * pow(1.f - VoH,5.f);
}


//下面为blinn_phong的微表面形式
//diffuse_color + (gloss + 2)/8 * dot(view,halfvec)^gloss * Fresnel(specular_color,dot(wi,wh))
//G：可见性函数是常量1，表示所有微表面上的光线完全没有自遮挡
//F：这里引入了一张高光贴图，即不同介质的反射系数都由贴图控制
//D：法线分布函数基于一个gloss系数控制
//注意标准的fresnel角度是入射方向与表面介质法线的夹角，但一般使用半角向量拟合表面法线。
//所以当使用半角向量时，使用入射角与使用出射角是等价的。


vec3 Blinn_Phong_BRDF(vec3 wi,vec3 wo){
	vec3 albedo = texture(bsdf.albedoTexture,fs_in.TexCoords).xyz;
	vec3 specular_color = texture(bsdf.specularTexture,fs_in.TexCoords).xyz;
	vec3 wh = normalize(wi + wo);
	float VoH = dot(wo,wh);
	vec3 Fr = F_Schlick(specular_color,VoH);
	vec3 diffusecolor = lambert_diffuse(albedo);
	vec3 specularcolor = (bsdf.gloss + 2.f)/8.f * pow(max(dot(wh,fs_in.Normal),0),bsdf.gloss) * Fr;
	return (vec3(1.f) - Fr) * diffusecolor + specularcolor;
}


void main(){
	vec3 wi = -normalize(directionaLight.dir);
	vec3 wo = normalize(viewPos - fs_in.FragPos);
	vec3 wh = normalize(wi + wo);
	float cosTheta = max(dot(fs_in.Normal,wi),0);
	vec3 result = Blinn_Phong_BRDF(wi,wo) * directionaLight.L * cosTheta;
	FragColor = vec4(result,1.f); //gamma矫正
}