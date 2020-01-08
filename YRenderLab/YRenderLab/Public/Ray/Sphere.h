#ifndef _YRENDER_RAY_SPHERE_H_
#define _YRENDER_RAY_SPHERE_H_


#include <Public/Ray/HitInfo.h>


	class Sphere : public HitGeometric {
	public:
		Sphere(){}
		Sphere(const Vector3& center, float radius) : center(center), radius(radius) {}
		virtual ~Sphere(){}
	public:
		virtual bool hit(const Ray& r, float Mint, float Maxt, HitInfo& hitinfo) override;
		
		static Vector3 GeneratorRandPoint();
	public:
		Vector3 center;
		float radius;
	};





#endif