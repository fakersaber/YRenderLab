#ifndef _WINDOWTOOL_H
#define _WINDOWTOOL_H

#include <windows.h>

namespace Yrender {
	namespace Window {
		//随便定义一个先
		enum class ErrorCode {
			INIT_WINDOW_FAILED = 1,
			INIT_INITGDI_FAILED = 2,
		};
		class WindowClass {
		public:
			static WindowClass* GetWindowInstance(const int width, const int height);

		public:
			WindowClass(const WindowClass& rhs) = delete;
			WindowClass& operator=(const WindowClass& rhs) = delete;
			WindowClass() = delete;
			void WindowRun();

		private:
			~WindowClass();
			WindowClass(const int width, const int height);
			static LRESULT WINAPI WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
			void InitWindowClass();
			void InitGDI();

		private:
			HWND hwnd;
			int WindowWidth;
			int WindowHeight;
			HDC Hdc;
			HBITMAP hBitmap;
			HBITMAP hOldBitmap;
			ErrorCode ExitCode;
		};
	}
}


#endif
