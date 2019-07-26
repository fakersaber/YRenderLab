#ifndef _WINDOWTOOL_H
#define _WINDOWTOOL_H

#include <windows.h>
#include <iostream>

#include "ErrorCode.hpp"


namespace YRender {

	class WindowClass {
	public:
		WindowClass(const WindowClass& rhs) = delete;
		WindowClass& operator=(const WindowClass& rhs) = delete;
		static WindowClass* GetWindow();

	public:
		void WindowRun();
		ErrorCode WindowInit(const int width, const int height);

	private:
		WindowClass();
		~WindowClass();
		void InitWindowClass();


	private:
		static void InitGDI();
		static LRESULT WINAPI WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HWND hwnd;
		int WindowWidth;
		int WindowHeight;
	};
}


#endif
