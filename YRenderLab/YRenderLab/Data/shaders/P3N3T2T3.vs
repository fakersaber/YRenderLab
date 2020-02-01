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

void main()
{
    //这是先转换左手的以z轴为上转换到y轴为上，然后再reverseZ换到右手系
    vec3 x = vec3(1.f,0.f,0.f);
    vec3 y = vec3(0.f,0.f,-1.f);
    vec3 z = vec3(0.f,1.f,0.f);

    mat3 TempModule = mat3(x,y,z);
    vec3 ScaleModule = iModelPos * vec3(0.2);
    vs_out.FragPos = TempModule * ScaleModule;
    vs_out.TexCoords = iTexCoords;


    mat3 normalMatrix = transpose(inverse(TempModule));
    vec3 N = normalize(normalMatrix * iModelNormal);
	vs_out.Normal = N;

    // vec3 T = normalize(normalMatrix * iModelTangent);
    // vs_out.Tangent = normalize(T - dot(T, N) * N);

    //法线计算都使用了模型的逆转置，那么切线变换矩阵一定是ModelToWorld
    vs_out.Tangent = normalize(TempModule * iModelTangent);

    gl_Position = projection * view * vec4(vs_out.FragPos,1.0f);

}