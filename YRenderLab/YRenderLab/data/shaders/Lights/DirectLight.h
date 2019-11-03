#ifndef _YRENDER_SHADER_LIGHT_DIRECT_H
#define _YRENDER_SHADER_LIGHT_DIRECT_H

#include "AmbientLight.h"

struct DirectLight{
    vec3 direction;
    vec3 lightColor;
}

uniform DirectLight _DirectLight;


#endif