#include <Public/Ray/Ray.h>


namespace YRender {
	//float Ray::HitSphere(const Vector3& center, const float radius) const {
	//	//t*t*B*B + 2*t*B*(A-C) + (A-C)*(A-C) = radius^2
	//	Vector3 TempVal = origin - center;
	//	float a = direct.Dot(direct);
	//	float b = 2 * direct.Dot(TempVal);
	//	float c = TempVal.Dot(TempVal) - radius * radius;
	//	float discriminant = b*b - 4*a*c;
	//	if (discriminant < 0.f) {
	//		return -1.f;
	//	}
	//	return (-b - sqrt(discriminant)) / (2.f*a);
	//}

}