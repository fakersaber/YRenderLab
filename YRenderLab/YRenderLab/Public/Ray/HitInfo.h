#ifndef _YRENDER_RAY_HITINFO_H_
#define _YRENDER_RAY_HITINFO_H_

#include <Public/Ray/Ray.h>




	struct HitInfo {
		float t;
		Vector3 HitPoint;
		Vector3 Normal;
	};


	class HitGeometric {
	public:
		virtual bool hit(const Ray& r, float Mint, float Maxt, HitInfo& hitinfo) = 0;
	};






#endif