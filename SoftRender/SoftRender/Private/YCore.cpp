#include <Public/YCore.h>

#define SOFT_RENDER
//#define OPENGL_RENDER

namespace YRender {

	YCore* YCore::GetCore() {
		static YCore instance;
		return &instance;
	}



	YCore::YCore() {
		this->_RenderWindow = RenderWindow::GetWindow();
#ifdef SOFT_RENDER
		this->_RenderClass = SoftRender::GetRender();
#elif OPENGL_RENDER
		this->_RenderClass = OpenGLRender::GetRender();
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

		if (!_RenderClass->Initial(_RenderWindow->GetHwnd(), width, height))
			return false;


		return true;
	}






	void YCore::Run() {
		MSG msg = { 0 };
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				_RenderClass->Tick();
				_RenderClass->Render();
			}
		}
	}


}
