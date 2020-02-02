#ifndef _YRENDER_BASIC_BSDF_H_
#define _YRENDER_BASIC_BSDF_H_

#include <Public/Basic/Material.h>


class Image;
class BSDF : public Material {
protected:
	BSDF() = default;
	virtual ~BSDF() = default;

public:
	//BSDF函数项(大多数继承只实现BRDF，即反射部分)
	/*
	@Param1 出射方向
	@Param2 入射方向
	@Param3 采样UV
	*/
	virtual const RGBf F(const Vector3& Wo, const Vector3& Wi, const Vector2& texcoord) = 0;

	// 概率密度函数，用于离线计算
	virtual float PDF(const Vector3& Wo, const Vector3& Wi, const Vector2& texcoord) = 0;

	virtual const RGBf GetEmission() const { return RGBf(0.f); }

public:
	//还是右手系y轴朝上
	static Vector3 TangentSpaceToWorld(const Vector3& worldTangenet, const Vector3& worldNormal, const Vector3& TangentSpaceNormal);
};




#endif