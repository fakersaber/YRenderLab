#ifndef _YRENDER_YCORE_H
#define _YRENDER_YCORE_H


#define NOMINMAX
#include <Public/RenderWindow.h>
#include <Public/SoftRHI/Win32Window.h>
#include <Public/SoftRHI/SoftRHI.h>


namespace YRender {
	class YCore {
	public:
		YCore(const YCore& rhs) = delete;
		YCore& operator=(const YCore& rhs) = delete;
		static YCore* GetCore();

	public:
		bool Initial(const int width, const int height);
		void Run();


	private:
		YCore();
		~YCore();


	private:
		RenderWindow* _RenderWindow;
		RenderClass* _RenderClass;

	};
}


#endif