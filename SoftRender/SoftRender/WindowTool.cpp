#include "WindowTool.h"


namespace Window {

	HWND hwnd = nullptr;
	int WindowWidth = 0;
	int WindowHeight = 0;

	LRESULT WINAPI WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {

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

	void WindowInit(const int& width, const int& height) {
		WindowWidth = width;
		WindowHeight = height;
		WNDCLASSEX WindowInfo = {0};
		WindowInfo.cbSize = sizeof(WNDCLASSEX);
		WindowInfo.hInstance = GetModuleHandle(nullptr);
		WindowInfo.hCursor = LoadCursor(nullptr, IDC_ARROW);
		WindowInfo.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1); //+1得到与菜单颜色不一样
		WindowInfo.style = CS_HREDRAW | CS_VREDRAW;
		WindowInfo.lpszClassName = L"YRender Window";
		WindowInfo.lpfnWndProc = WndProc;

		RegisterClassEx(&WindowInfo);
		RECT rc = { 0, 0, width, height };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

		hwnd = CreateWindowEx(WS_EX_APPWINDOW,
			WindowInfo.lpszClassName, WindowInfo.lpszClassName,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, WindowInfo.hInstance, nullptr);
		ShowWindow(hwnd, SW_SHOW);
		MSG msg = { 0 };
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			//

		}
	}
}