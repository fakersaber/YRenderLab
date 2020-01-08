#ifndef _YRENDER_RAY_RAYCAMERA_H_
#define _YRENDER_RAY_RAYCAMERA_H_

#include <Public/Ray/Ray.h>


	class RayCamera {
	public:
		RayCamera(
			const Vector3& lowLeftCorner = Vector3(-2.f,-1.f,-1.f),
			const Vector3& horizontal = Vector3(4.f,0.f,0.f),
			const Vector3& vertical = Vector3(0.f,2.f,0.f),
			const Vector3& position = Vector3(0.f,0.f,0.f)
		);

		Ray& GetRay(float u, float v) {
			ray.SetOrigin(position);
			ray.SetDirection(LowLeft + horizontal * u + vertical * v);
			return ray;
		}

		static void TestRayTracer();

	public:
		Vector3 position;
		Vector3 LowLeft;
		Vector3 horizontal;
		Vector3 vertical;
		Ray ray;
	};





#endif