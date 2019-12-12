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
}


#endif