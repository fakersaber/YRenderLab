#ifndef _YRENDER_SOFTRHI_WIN32WINDOW_H
#define _YRENDER_SOFTRHI_WIN32WINDOW_H


#include <Public/RenderWindow.h>
#include <Public/SoftRHI/SoftRHI.h>

#include <windows.h>

	class Win32Window : public RenderWindow
	{
	public:
		Win32Window(const Win32Window& rhs) = delete;
		Win32Window& operator=(const Win32Window& rhs) = delete;

	public:
		virtual bool Initial(const int width, const int height) override;
		virtual void Run() override;
		static Win32Window* GetInstance();

	private:
		static LRESULT WINAPI WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		Win32Window();
		virtual ~Win32Window();

	private:
		//增加渲染线程后可以分开，现在暂时不能分开，渲染在窗体循环里面
		std::unique_ptr<RenderClass> _RenderClass;
		HWND hwnd;	
	};




#endif