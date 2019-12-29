#version 330 core
out vec2 FragColor;
in vec2 TexCoords;

#define TWO_PI        6.28318530718

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
// ----------------------------------------------------------------------------
vec2 Hammersley(uint i, uint N)
{
	return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

// ----------------------------------------------------------------------------
vec3 Trowbridge_Reitz_GGX_Sample(vec2 Xi, vec3 N, float roughness){
    float a = roughness * roughness;;
    float phi = TWO_PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.f - cosTheta * cosTheta);

    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

	// from tangent-space H vector to world-space sample vector
	vec3 up          = vec3(0.0, 1.0, 0.0);
	vec3 tangent   = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);
	
	vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
	return normalize(sampleVec);
}
// ----------------------------------------------------------------------------
float SchlickGGX_G1(vec3 norm, vec3 w, float roughness) {
	float alpha = roughness * roughness;
	float k = alpha / 2.0; // fix smith model ggx   IBL
	
	float NoW = max(0.0, dot(norm, w));
	return NoW / (NoW * (1.0 - k) + k);
}
// ----------------------------------------------------------------------------
float SchlickGGX_Smith_G(vec3 N, vec3 wo, vec3 wi, float roughness){
    return SchlickGGX_G1(N, wo, roughness) * SchlickGGX_G1(N, wi, roughness);
}
// ----------------------------------------------------------------------------
vec2 IntegrateBRDF(float NdotV, float roughness){
    //use uv as cos\theta and roughness
    vec3 V;
    V.x = sqrt(1.0 - NdotV * NdotV); //sin \theta
    V.y = 0.0;
    V.z = NdotV; 

    //现在只用到两个通道存储值
    float Red = 0.0;
    float Green = 0.0;

    vec3 N = vec3(0.0,0.0,1.0);

    const uint SAMPLE_COUNT = 1024u;
    for(uint i = 0u; i < SAMPLE_COUNT; ++i){
        vec2 Xi = Hammersley(i,SAMPLE_COUNT);
        vec3 H = Trowbridge_Reitz_GGX_Sample(Xi,N,roughness);
        vec3 L = normalize(2 * dot(H,V) * H - V); //貌似不需要normalize，因为V是单位向量，待定
        
        float NdotL = max(L.z,0.0);
        float NdotH = max(H.z,0.0);
        float VdotH = max(dot(H,V),0.0);

        if(NdotL > 0.0){
            float G = SchlickGGX_Smith_G(N,V,L,roughness);
            float G_Val = (VdotH * G)/(NdotV * NdotH);
            float Fc = pow(1.0 - VdotH, 5.0);
            
            Red += (1.0 - Fc) * G_Val;
            Green += Fc * G_Val;
        }

    }
    return vec2(Red, Green) / float(SAMPLE_COUNT);
}
// ----------------------------------------------------------------------------
void main() 
{
    FragColor = IntegrateBRDF(TexCoords.x, TexCoords.y);
}