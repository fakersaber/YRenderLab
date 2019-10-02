#ifndef _YRENDER_RAY_RAY_H
#define _YRENDER_RAY_RAY_H

#include <Public/YGM/Vector3.hpp>


namespace YRender {
	// y = origin + t * direction
	class Ray {
	public:
		Ray();
		~Ray();
		explicit Ray(const Vector3& origin) : A(origin) {}
		Ray(const Vector3& origin, const Vector3& direction)
			:A(origin), B(direction) {}
	public:
		const Vector3 origin() const { return A; }
		const Vector3 direction() const { return B; }
		void SetDirection(const Vector3& direction) { B = direction; }
		void SetOrigin(const Vector3& origin) { A = origin; }
		Vector3 PointAtParameter(float t) const { return A + B * t; }

		static void TestRayTracer();

	private:
		Vector3 A;
		Vector3 B;
	};
}





#endif