#version 330 core

layout (location = 0) out vec4 GBuffer0;
layout (location = 1) out vec4 GBuffer1;
layout (location = 2) out vec4 GBuffer2;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
} vs_out;


struct BSDF_StandardPBR {
	vec3 colorFactor;
    sampler2D albedoTexture;

	float metallicFactor;
    sampler2D metallicTexture;

	float roughnessFactor;
    sampler2D roughnessTexture;

    sampler2D aoTexture;
	sampler2D normalTexture;
};

uniform BSDF_StandardPBR Material;

vec3 CalcBumpedNormal(vec3 normal, vec3 tangent, sampler2D normalTexture, vec2 texcoord){
    tangent = normalize(tangent - normal *　dot(tangent,normal));
    vec3 bitangent = cross(tangent,nromal);
    mat3 TBN = mat3(tangent,bitangent,normal);
    vec3 BumpNormal = 2.0 * texture(normalTexture,texcoord) - 1.0;
    vec3 newNormal = normalize(TBN * BumpNormal);
    return newNormal;
}

void main(){
    vec3 albedo = texture(Material.albedoTexture,vs_out.TexCoords) * Material.colorFactor;
    float metallic = texture(bsdf.metallicTexture,fs_in.TexCoords).x * bsdf.metallicFactor;
    float roughness = texture(bsdf.roughnessTexture,fs_in.TexCoords).x * bsdf.roughnessFactor;
    float ao = texture(bsdf.aoTexture,fs_in.TexCoords).x;
    vec3 normal = CalcBumpedNormal(normalize(fs_in.Normal), normalize(fs_in.Tangent), Material.normalTexture, fs_in.TexCoords);

    GBuffer0 = vec4(fs_in.FragPos,roughness);
    GBuffer1 = vec4(normal,metallic);
    GBuffer2 = vec4(albedo,ao);
}