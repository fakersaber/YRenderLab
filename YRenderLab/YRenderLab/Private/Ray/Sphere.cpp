#include <Public/Ray/Sphere.h>


namespace YRender {
	bool Sphere::hit(const Ray& r, float Mint, float Maxt, HitInfo& hitinfo) {
		//在此求根可以令b = r.GetDirection().Dot(TempVal)，提出2约掉
		Vector3 TempVal = r.GetOrigin() - center;
		float a = r.GetDirection().Dot(r.GetDirection());
		float b = r.GetDirection().Dot(TempVal);
		float c = TempVal.Dot(TempVal) - radius * radius;
		float discriminant = b * b - a * c;
		if (discriminant > 0.f) {
			//这只能保证在正方向的closest,不过假设摄像机在原点的情况下可以只考虑正方向
			float solution = (-b - sqrt(discriminant)) / a;
			if (solution < Maxt && solution > Mint) {
				hitinfo.t = solution;
				hitinfo.HitPoint = r.PointAtParameter(solution);
				hitinfo.Normal = (hitinfo.HitPoint - center) / radius;
				return true;
			}
			solution = (-b + sqrt(discriminant)) / a;
			if (solution < Maxt && solution > Mint) {
				hitinfo.t = solution;
				hitinfo.HitPoint = r.PointAtParameter(solution);
				hitinfo.Normal = (hitinfo.HitPoint - center) / radius;
				return true;
			}
		}
		return false;
	}
}