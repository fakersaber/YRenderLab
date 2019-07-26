#include "RenderWindow.h"



namespace YRender {
	void* FrameBuffer = nullptr;

	RenderWindow* RenderWindow::GetWindow()
	{
		static RenderWindow instance;
		return &instance;
	}

	RenderWindow::RenderWindow() : hwnd(nullptr){}


	RenderWindow::~RenderWindow() {}

	bool RenderWindow::Initial(const int width, const int height) {
		WNDCLASSEX WindowInfo = { 0 };
		WindowInfo.cbSize = sizeof(WNDCLASSEX);
		WindowInfo.hInstance = GetModuleHandle(nullptr);
		WindowInfo.hCursor = LoadCursor(nullptr, IDC_ARROW);
		WindowInfo.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1); //+1 got diffrent color with the color of mune
		WindowInfo.style = CS_HREDRAW | CS_VREDRAW;
		WindowInfo.lpfnWndProc = WndProc;
		WindowInfo.lpszClassName = L"YRender";
		RegisterClassEx(&WindowInfo);
		RECT rc = { 0, 0, width, height };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
		hwnd = CreateWindowEx(WS_EX_APPWINDOW,
			WindowInfo.lpszClassName, WindowInfo.lpszClassName,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, WindowInfo.hInstance, nullptr);
		if (!hwnd)
			return false;
		ShowWindow(hwnd, SW_SHOW);
		return true;
	}


	LRESULT WINAPI RenderWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message)
		{
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}

		case WM_ERASEBKGND:
			return true;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
}