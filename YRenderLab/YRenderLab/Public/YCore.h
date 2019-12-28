#ifndef _YRENDER_YCORE_H
#define _YRENDER_YCORE_H


#define NOMINMAX

//#include <Public/SoftRHI/SoftRHI.h>
//#include <Public/Ray/RayCamera.h>
#include <memory>

namespace YRender {
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
		RenderWindow* _RenderWindow;
	};



	namespace CoreDefine {
		const float data_ScreenVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		const float data_Flip_ScreenVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 0.0f,
			-1.0f, -1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 1.0f,

			-1.0f,  1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 1.0f,
			 1.0f,  1.0f,  1.0f, 0.0f
		};
	}
}


#endif