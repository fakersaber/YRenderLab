#version 330 core

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

void main()
{
    //model matrix set identity temply
    vs_out.FragPos = iModelPos;
    vs_out.Normal = iModelNormal;
    vs_out.TexCoords = iTexCoords;
    vs_out.Tangent = iModelTangent;
    gl_Position = projection * view * vec4(vs_out.FragPos,1.0f);

}