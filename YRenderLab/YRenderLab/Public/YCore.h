#ifndef _YRENDER_YCORE_H
#define _YRENDER_YCORE_H


#define NOMINMAX
#define YRENDER_REVERSE

#include <memory>

#include <Public/YGM/Transform.h>

class GlfwWindow;
class RenderWindow;

class YCore {
public:
	YCore(const YCore& rhs) = delete;
	YCore& operator=(const YCore& rhs) = delete;
	static YCore* GetCore();
	bool Initial(const int width, const int height);
	void Run();
	std::shared_ptr<GlfwWindow> GetGLWindow() const;


private:
	YCore();
	~YCore();


private:
	std::shared_ptr<GlfwWindow> pGLInstance;

	//#TODO remove
	RenderWindow* _RenderWindow;
};



namespace CoreDefine {
	enum class StaticVAOType : uint8_t {
		Sphere,
		Plane,
		Screen,
		Cube
	};

	static const float data_ScreenVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	static const float data_Flip_ScreenVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 0.0f,
		-1.0f, -1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 1.0f,

		-1.0f,  1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 0.0f
	};

	static const YGM::Transform ModelReverse = YGM::Transform(
		Vector4(1.f, 0.f, 0.f, 0.f),
		Vector4(0.f, 0.f, 1.f, 0.f),
		Vector4(0.f, 1.f, 0.f, 0.f),
		Vector4(0.f, 0.f, 0.f, 1.f)
	);

	//上面模型转换显然是正交矩阵，逆矩阵与原矩阵相同
	//static const YGM::Transform InverseModelReverse = YGM::Transform(
	//	Vector4(1.f, 0.f, 0.f, 0.f),
	//	Vector4(0.f, 0.f, 1.f, 0.f),
	//	Vector4(0.f, 1.f, 0.f, 0.f),
	//	Vector4(0.f, 0.f, 0.f, 1.f)
	//);

	static const int maxPointLights = 0;

	static const int maxDirectionalLights = 1;

	static const int maxSpotLights = 0;


}


#endif