#pragma once

#include <memory>
#include <Public/YGM/Transform.h>

#define NOMINMAX

#define YRENDER_REVERSE

//Window Define
#define USE_GLFWWINDOW 1

//RHI Define
#define SOFT_RENDER 0
#define OPENGL_RENDER 0
#define VULKAN_RENDER 1

//Platform Define
#define USE_WINDOWS_PLATFORM 1

namespace CoreDefine {
	enum class StaticVAOType : uint8_t {
		Sphere,
		Plane,
		Screen,
		Cube
	};

	enum class MaterialID : uint8_t {
		BSDF_StandardPBR = 0,
		BSDF_Emission = 1
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

	//显然是正交矩阵，逆矩阵与原矩阵相同
	static const YGM::Transform ModelReverse = YGM::Transform(
		Vector4(1.f, 0.f, 0.f, 0.f),
		Vector4(0.f, 0.f, 1.f, 0.f),
		Vector4(0.f, 1.f, 0.f, 0.f),
		Vector4(0.f, 0.f, 0.f, 1.f)
	);

	constexpr static int maxPointLights = 0;

	constexpr static int maxDirectionalLights = 1;

	constexpr static int maxSpotLights = 0;

	const static std::string AssetPath = "Data/";

}