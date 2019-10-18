#include <Public/Ray/Sphere.h>
#include <random>

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

	//r^2*sinα*dr*dα*dγ  球的微分体积表示

	//所以随机生成球上面一点，积分为r^2dr ~ r^3, sinαdα ~ -cosα, dγ ~ γ

	//所以三个值分别为 [ r的1/3次方，-cosα，γ]服从均匀分布，均匀分布生成[-1,1]，
	
	//在Cpp中闭区间的浮点生成使用 dis2(0, std::nextafter(1.f, DBL_MAX))，

	//终值生成1.f的下个浮点表示

	Vector3 Sphere::GeneratorRandPoint() {
		std::random_device rd;
		std::mt19937 gen(rd());

		//-costheta,[-1,1]之间均匀分布
		std::uniform_real_distribution<float> disTheta(-1.f, std::nextafter(1.f, FLT_MAX));

		//gama, [0,2PI]之间均匀分布
		std::uniform_real_distribution<float> disGamma(0.f, std::nextafter(2.f * YGM::Math::PI, FLT_MAX));

		//radius^3, [0,r]之间均匀分布 
		std::uniform_real_distribution<float> Radius(0.f, std::nextafter(1.f, FLT_MAX));

		auto r = pow(Radius(gen),0.333333f);
		auto theta = acos(disTheta(gen));
		auto garma = disGamma(gen);

		//auto test = pow(r * cos(garma) * sin(theta), 2) + pow(r * sin(garma) * sin(theta), 2) + pow(r * cos(theta), 2);
		//std::cout << test << std::endl;

		return Vector3
		(
			r * cos(garma) * sin(theta),
			r * sin(garma) * sin(theta),
			r * cos(theta)
		);

	}
}