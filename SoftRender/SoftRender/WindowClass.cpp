#include "WindowClass.h"

#include <iostream>

namespace Yrender {
	namespace Window {
		unsigned char* FrameBuffer[1024];

		WindowClass* WindowClass::GetWindowInstance(const int width, const int height)
		{
			static WindowClass* instance = new WindowClass(width, height);
			return instance;
		}

		WindowClass::WindowClass(const int width, const int height) :
			WindowWidth(width), WindowHeight(height), hwnd(nullptr), ExitCode(static_cast<ErrorCode>(0))
		{
			InitWindowClass();
			if (ExitCode == ErrorCode::INIT_WINDOW_FAILED) {
				std::cout << "Create Window failed" << std::endl;
				return;
			}
			InitGDI();
			if (ExitCode == ErrorCode::INIT_WINDOW_FAILED) {
				std::cout << "Create DIB failed" << std::endl;
				return;
			}
		}

		WindowClass::~WindowClass() {

			//Realease DC
			SelectObject(Hdc, hOldBitmap);
			DeleteObject(hBitmap);
			DeleteDC(Hdc);
		}

		void WindowClass::WindowRun() {
			MSG msg = { 0 };
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}

		LRESULT WINAPI WindowClass::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
			switch (message)
			{
			case WM_DESTROY:
				PostQuitMessage(0);
				break;

			case WM_PAINT:
				break;

			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			return 0;
		}

		void WindowClass::InitWindowClass() {
			WNDCLASSEX WindowInfo = { 0 };
			WindowInfo.cbSize = sizeof(WNDCLASSEX);
			WindowInfo.hInstance = GetModuleHandle(nullptr);
			WindowInfo.hCursor = LoadCursor(nullptr, IDC_ARROW);
			WindowInfo.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1); //+1得到与菜单颜色不一样
			WindowInfo.style = CS_HREDRAW | CS_VREDRAW;
			WindowInfo.lpszClassName = L"YRender Window";
			WindowInfo.lpfnWndProc = WndProc;

			RegisterClassEx(&WindowInfo);
			RECT rc = { 0, 0, WindowWidth, WindowHeight };
			AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

			hwnd = CreateWindowEx(WS_EX_APPWINDOW,
				WindowInfo.lpszClassName, WindowInfo.lpszClassName,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, WindowInfo.hInstance, nullptr);

			if (hwnd == nullptr) {
				ExitCode = static_cast<ErrorCode>(0);
				return;
			}
			ShowWindow(hwnd, SW_SHOW);
		}

		void WindowClass::InitGDI() {
			HDC CurDC = GetDC(hwnd);
			this->Hdc = CreateCompatibleDC(CurDC);
			ReleaseDC(hwnd, CurDC);
			BITMAPINFO bi =
			{
				sizeof(BITMAPINFOHEADER), WindowWidth, -WindowHeight,
				1, 32, BI_RGB,WindowWidth * WindowHeight * 4, 0, 0, 0, 0
			};
			if (CreateDIBSection(this->Hdc, &bi, DIB_RGB_COLORS, reinterpret_cast<void**>(FrameBuffer), nullptr, 0) == nullptr)
			{
				ExitCode = static_cast<ErrorCode>(0);
				return;
			}
			hOldBitmap = reinterpret_cast<HBITMAP>(SelectObject(this->Hdc, hBitmap));
		}
	}
}