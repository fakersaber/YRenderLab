#version 430 core

#define PI            3.14159265359
#define TWO_PI        6.28318530718

out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;
uniform float roughness;
uniform float resolution;


vec3 Trowbridge_Reitz_GGX_Sample(vec2 Xi, vec3 N, float roughness){
    float a = roughness * roughness;
    float phi = TWO_PI * Xi.x;
    float cosTheta = sqrt((1.f - Xi.y)/(1.f + (a*a - 1.f)*Xi.y));
    float sinTheta = sqrt(1.f - cosTheta * cosTheta);

    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    //可以证明总有对应的向量使得值相同，当up值选取不同的时候
    vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);
	
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}

float Trowbridge_Reitz_GGX_D(vec3 norm, vec3 h, float roughness){
	float NoH = max(dot(norm, h),0.0);
	float alpha = roughness * roughness;
	float alpha2 = alpha * alpha;
	float cos2Theta = NoH * NoH;
	float t = (alpha2 - 1) * cos2Theta + 1;
	return alpha2 / (PI * t * t);
}




float RadicalInverse_VdC(uint bits) {
	// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
	// efficient VanDerCorpus calculation.

	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 Hammersley(uint i, uint N) {
	return vec2(float(i) / float(N), RadicalInverse_VdC(i));
}


void main(){
    //comme form paper Krivanek2008
    //float ConstK = 6.f / (4.f * PI);
    //float PrefilterApprox = resolution * resolution * ConstK;
    float SolidAngleTexel = 4.f * PI / (6.f * resolution * resolution);
    vec3 N = normalize(WorldPos);
    //make the simplyfying assumption that V equals R equals the normal 
    vec3 R = N;
    vec3 V = R;

    const uint SAMPLE_COUNT = 1024u;
    vec3 prefilteredColor = vec3(0.0);
    float DownValue = 0.0;
    for(uint i = 0u; i < SAMPLE_COUNT; ++i){
        //use low-discrepancy sequence generates random value
        vec2 Xi = Hammersley(i,SAMPLE_COUNT);
        vec3 H = Trowbridge_Reitz_GGX_Sample(Xi,N,roughness);
        vec3 L = normalize(2.0 * dot(H,V) * H - V);
        float NdotL = max(dot(N,L),0.0);
        if(NdotL > 0.0){
            float D = Trowbridge_Reitz_GGX_D(N,H,roughness);
            float NdotH = max(dot(N,H),0.0);
            float HdotV = max(dot(H,V),0.0);
            float pdf = D * NdotH / (4.0 * HdotV)+ 0.0001; 
            //+0.0001防止除0
            float SolidAngleSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);
            float mipLevel =  roughness == 0.0 ? 0.0 : 0.5 * log2(SolidAngleSample / SolidAngleTexel);

            prefilteredColor += textureLod(environmentMap,L,mipLevel).rgb * NdotL;
            DownValue += NdotL;
        }
    }
    prefilteredColor = prefilteredColor / DownValue;
    FragColor = vec4(prefilteredColor,1.0);
}