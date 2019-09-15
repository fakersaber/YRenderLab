#include <Public/SoftRHI/Win32Window.h>



namespace YRender {

	Win32Window::Win32Window() {}

	Win32Window::~Win32Window() {};

	bool Win32Window::Initial(const int width, const int height) {
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
		this->_RenderClass = new SoftRender(hwnd);
		if (!_RenderClass->Initial(width, height)) {
			delete _RenderClass;
			return false;
		}
		return true;
	}

	void Win32Window::Run() {
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


	LRESULT WINAPI Win32Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message)
		{
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
						 //禁止背景擦除,但是这样不会清除相关绘制区域
		case WM_ERASEBKGND:
			return true;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
}