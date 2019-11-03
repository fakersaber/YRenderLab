#ifndef _YRENDER_SHADER_LIGHT_AMBIENT_H
#define _YRENDER_SHADER_LIGHT_AMBIENT_H

struct AmbientLight{
    vec3 lightColor;
    float intensity;
}

uniform AmbientLight _AmbientLight;


#endif