#ifndef _YRENDER_LIGHTS_DIRECTIONLIGHT_H_
#define _YRENDER_LIGHTS_DIRECTIONLIGHT_H_

#include <Public/Lights/Light.h>
#include <Public/YGM/Vector3.hpp>
#include <Public/YGM/RGB.hpp>

class DirectionalLight : public Light {
public:
	DirectionalLight(RGBf Color = RGBf::White, float intensity = 1.f)
		:
		Color(Color),intensity(intensity) 
	{

	}

protected:
	virtual ~DirectionalLight() = default;
	DirectionalLight() = delete;

public:
	float intensity;
	RGBf Color;
};



#endif