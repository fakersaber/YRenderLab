#ifndef _YRENDER_BASIC_BSDF_EMISSION_H_
#define _YRENDER_BASIC_BSDF_EMISSION_H_

#include <Public/Basic/BSDF.h>

class Raster;

class BSDF_Emission : public BSDF {

public:
	BSDF_Emission(const RGBf& color = RGBf(1.f), float intensity = 1.f)
		: color(color), intensity(intensity) 
	{

	}
protected:
	virtual ~BSDF_Emission() = default;

public:
	//BSDFÏî
	virtual const RGBf F(const Vector3& Wo, const Vector3& Wi, const Vector2& texcood) override;

	virtual float PDF(const Vector3& Wo, const Vector3& Wi, const Vector2& texcoord) override;

	virtual void SetCurMaterial(const std::shared_ptr<Raster>& Raster) override;


public:
	RGBf color;
	float intensity;
};



#endif