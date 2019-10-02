#include <Public/Ray/Ray.h>
#include <Public/Basic/Image/Image.h>
#include <Public/YGM/RGB.hpp>


namespace YRender {
	Ray::Ray() {}

	Ray::~Ray() {}


	void Ray::TestRayTracer() {
		int width = 400;
		int height = 200;
		Vector3 LowLeftCorner(-2.0f, -1.0f, -1.0f);
		Vector3 HorizonVec(4.0f, 0.f, 0.f);
		Vector3 VerticalVec(0.f, 2.f, 0.f);
		Vector3 origin(0.f);
		Ray ray(origin);
		uint8_t* OutPutData = new uint8_t[width * height * 3];
		RGBf EndColor(0.5f, 0.7f, 1.f);
		for (int j = 0; j < height; ++j) {
			for (int i = 0; i < width * 3; i += 3) {
				auto direction = LowLeftCorner + HorizonVec * (i / static_cast<float>(width)) + VerticalVec * (j / static_cast<float>(height)) - origin;
				ray.SetDirection(direction.Normalize());
				float t = (ray.direction().y + 1.0f) * 0.5f;
				auto Color = RGBf::White * (1.f - t) + EndColor * t;

				//左下角作为起点，stb倒过来
				OutPutData[i + (height - 1 - j) * width * 3] = static_cast<uint8_t>(Color.r * 255.f);
				OutPutData[i + (height - 1 - j) * width * 3 + 1] = static_cast<uint8_t>(Color.g * 255.f);
				OutPutData[i + (height - 1 - j) * width * 3 + 2] = static_cast<uint8_t>(Color.b * 255.f);
			}
		}
		Image::OutPng("C:\\Users\\Administrator\\Desktop\\ray.png", width, height, 3, OutPutData);
		delete OutPutData;
	}
}