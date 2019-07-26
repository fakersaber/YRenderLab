#include "WindowClass.h"
#include "RenderDevice.h"


namespace YRender {
	void* FrameBuffer = nullptr;

	WindowClass* WindowClass::GetWindow()
	{
		static WindowClass instance;
		return &instance;
	}

	WindowClass::WindowClass() :
		hwnd(nullptr)
	{

	}

	WindowClass::~WindowClass() {

	}

	ErrorCode WindowClass::WindowInit(const int width, const int height) {
		{
			this->WindowWidth = width;
			this->WindowHeight = height;
			WNDCLASSEX WindowInfo = { 0 };
			WindowInfo.cbSize = sizeof(WNDCLASSEX);
			WindowInfo.hInstance = GetModuleHandle(nullptr);
			WindowInfo.hCursor = LoadCursor(nullptr, IDC_ARROW);
			WindowInfo.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1); //+1得到与菜单颜色不一样
			WindowInfo.style = CS_HREDRAW | CS_VREDRAW;
			WindowInfo.lpfnWndProc = WndProc;
			WindowInfo.lpszClassName = L"YRender";
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
				std::cout << "Create Window Failed" << std::endl;
				return ErrorCode::INIT_WINDOW_FAILED;
			}
			ShowWindow(hwnd, SW_SHOW);
		}

		{
			return RenderDevice::GetDevice()->Initial(this->hwnd, width, height);
		}

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
			else 
			{
				RenderDevice::GetDevice()->Draw();
			}
		}
	}



	LRESULT WINAPI WindowClass::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message)
		{
		case WM_CREATE: {
			break;
		}
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