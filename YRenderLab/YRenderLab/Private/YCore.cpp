#include <Public/YCore.h>

#define SOFT_RENDER
//#define OPENGL_RENDER

namespace YRender {

	YCore* YCore::GetCore() {
		static YCore instance;
		return &instance;
	}



	YCore::YCore() {
		#ifdef SOFT_RENDER
			this->_RenderWindow = Win32Window::GetGetInstance();
		#elif OPENGL_RENDER
			this->_RenderWindow = new GlfwWindow();
		#endif 
	}

	YCore::~YCore() {

	}

	bool YCore::Initial(const int width, const int height) {

		if (!_RenderWindow->Initial(width, height))
		{
			std::cout << "Create Window Failed" << std::endl;
			return false;
		}
		return true;
	}

	void YCore::Run() {
		_RenderWindow->Run();
	}


}
