#include <Public/SoftRHI/Win32Window.h>
#include <thread>


namespace YRender {
	enum Win32KeyBoard
	{
		W = 0x57,
		A = 0x41,
		S = 0x53,
		D = 0x44,
		Q = 0x51,
		E = 0x45
	};

	Win32Window* Win32Window::GetInstance() {
		static Win32Window instance;
		return &instance;
	}

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
		//std::chrono::time_point<std::chrono::high_resolution_clock> begin;
		//int64_t deltaTime = 0;
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//begin = std::chrono::high_resolution_clock::now();
				_RenderClass->Tick();
				_RenderClass->Render();
				//deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - begin).count();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
		//case WM_ERASEBKGND:
		//	return true;

		case WM_KEYDOWN:
			switch (wParam)
			{
			case Win32KeyBoard::W:
				Win32Window::GetInstance()->_RenderClass->GetCamera().ProcessKeyboard(Camera::ENUM_Movement::MOVE_FORWARD);
				break;
			case Win32KeyBoard::S:
				Win32Window::GetInstance()->_RenderClass->GetCamera().ProcessKeyboard(Camera::ENUM_Movement::MOVE_BACKWARD);
				break;
			case Win32KeyBoard::A:
				Win32Window::GetInstance()->_RenderClass->GetCamera().ProcessKeyboard(Camera::ENUM_Movement::MOVE_LEFT);
				break;
			case Win32KeyBoard::D:
				Win32Window::GetInstance()->_RenderClass->GetCamera().ProcessKeyboard(Camera::ENUM_Movement::MOVE_RIGHT);
				break;
			case Win32KeyBoard::Q:
				Win32Window::GetInstance()->_RenderClass->GetCamera().ProcessKeyboard(Camera::ENUM_Movement::MOVE_UP);
				break;
			case Win32KeyBoard::E:
				Win32Window::GetInstance()->_RenderClass->GetCamera().ProcessKeyboard(Camera::ENUM_Movement::MOVE_DOWN);
				break;
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
}