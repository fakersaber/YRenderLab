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

out vec3 TexCoords;

//NDC左手系 Z轴朝屏幕里面所以-1为最近
void main()
{
    TexCoords = iModelPos;
	mat4 viewNoT = mat4(mat3(view));
    vec4 pos = projection * viewNoT * vec4(iModelPos,1.0f);
    gl_Position = pos.xyww;

}