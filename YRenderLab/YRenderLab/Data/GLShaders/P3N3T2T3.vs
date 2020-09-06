#version 430 core

// 160，总大小按vec4对齐（必须是vec4倍数） but padded to a multiple of the size of a vec4.
layout (std140) uniform Camera{
	mat4 view;			// 64	0	
	mat4 projection;	// 64	64	
	vec3 viewPos;		// 12	128
	float nearPlane;	// 4	140	 //float按4字节对齐，140
	float farPlane;		// 4	144	
	float fov;			// 4	148	
	float ar;			// 4	152	
};

layout (location = 0) in vec3 iModelPos;
layout (location = 1) in vec3 iModelNormal;
layout (location = 2) in vec2 iTexCoords;
layout (location = 3) in vec3 iModelTangent;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
} vs_out;

uniform mat4 model;

void main()
{
    vec4 worldPos = model * vec4(iModelPos,1.0);
    vs_out.FragPos = worldPos.xyz/worldPos.w;
    vs_out.TexCoords = iTexCoords;
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 N = normalize(normalMatrix * iModelNormal);
	vs_out.Normal = N;
    vec3 T = normalize(normalMatrix * iModelTangent);
    vs_out.Tangent = normalize(T - dot(T, N) * N);
    //vs_out.Tangent = normalize(model * iModelTangent);
    gl_Position = projection * view * worldPos;

}