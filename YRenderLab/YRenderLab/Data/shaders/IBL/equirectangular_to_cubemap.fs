#version 330

#define PI            3.14159265359
#define TWO_PI        6.28318530718
#define FOUR_PI       12.56637061436
#define FOUR_PI2      39.47841760436
#define INV_PI        0.31830988618
#define INV_TWO_PI    0.15915494309
#define INV_FOUR_PI   0.07957747155
#define HALF_PI       1.57079632679
#define INV_HALF_PI   0.636619772367

out vec4 FragColor;
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D equirectangularMap;

float CaculatePhi(float x, float z);

vec2 TexcoordOf(vec3 dir);


void main(){
    vec2 uv = TexcoordOf(normalize(fs_in.FragPos));
    vec3 color = texture(equirectangularMap,uv).rgb;

    FragColor = vec4(color,1.0);
}


float CaculatePhi(float x, float z){
    if(z > 0){
        if(x >=0){
            return atan(x/z);
        }
        else{
            return atan(x/z) + TWO_PI;
        }
    }
    else if(z < 0){
        return atan(x/z) + PI;
    }
    else
        return PI + sign(x) * HALF_PI;
}

vec2 TexcoordOf(vec3 dir){
    float phi = CaculatePhi(dir.x,dir.z);
    float theta = acos(dir.y);

    float u = phi * INV_TWO_PI;
    float v = theta * INV_PI;
    return vec2(u,v);
}