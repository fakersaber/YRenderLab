#ifndef _YRENDER_RAY_RAY_H_
#define _YRENDER_RAY_RAY_H_

#include <Public/YGM/Vector3.hpp>



	// y = origin + t * direction
	class Ray {
	public:
		Ray() : origin(0.f, 0.f, 0.f) {};

		~Ray() {};

		explicit Ray(const Vector3& origin) : origin(origin) {}

		Ray(const Vector3& origin, const Vector3& direction)
			:origin(origin), direct(direction) {}

	public:
		const Vector3 GetOrigin() const { return origin; }
		const Vector3 GetDirection() const { return direct; }
		void SetDirection(const Vector3& direction) { this->direct = direction; }
		void SetOrigin(const Vector3& origin) { this->origin = origin; }
		Vector3 PointAtParameter(float t) const { return this->origin + direct * t; }




	//public:
	//	float HitSphere(const Vector3& center,const float radius) const;
	private:
		Vector3 origin;
		Vector3 direct;
	};






#endif