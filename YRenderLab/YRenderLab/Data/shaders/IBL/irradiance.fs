#version 330 core
#define PI            3.14159265359
#define TWO_PI        6.28318530718
#define INV_TWO_PI    0.15915494309
#define HALF_PI       1.57079632679

out vec4 FragColor;

in vec3 WorldPos;

uniform samplerCube environmentMap;

void main(){
    vec3 N = normalize(WorldPos);
    vec3 irradiance = vec3(0.f);

    //如果N方向在竖直方向采样结果应该是错误的。 需要修改为四元数旋转
    vec3 up = vec3(0.f,1.f,0.f);
    vec3 right = normalize(cross(up,N));
    up = cross(N,right);

    float sampleDelta = 0.025f;
    float nrSamples = 0.f;
    for(float phi = 0.f; phi < TWO_PI; phi += sampleDelta){
        for(float theta = 0.f; theta <HALF_PI; theta += sampleDelta){
            //将采样出的向量映射到以N为front的新坐标系下
            vec3 OriginSampleDir = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 CurSampleDir = right * OriginSampleDir.x + up * OriginSampleDir.y + N * OriginSampleDir.z;


            //注意没有tone mapping
            irradiance += texture(environmentMap,CurSampleDir).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    //1/pi一起运算
    irradiance = PI * irradiance * (1.f/nrSamples);
    FragColor = vec4(irradiance,1.f);
}

