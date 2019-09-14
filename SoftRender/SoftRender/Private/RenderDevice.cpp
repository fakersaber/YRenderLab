#include <Public/RenderDevice.h>



namespace YRender{

	RenderDevice* RenderDevice::GetDevice() {
		static RenderDevice instance;
		return &instance;
	}

	RenderDevice::RenderDevice():
		BufferHdc(nullptr),
		hBitmap(nullptr), 
		hOldBitmap(nullptr)
	{ }

	RenderDevice::~RenderDevice() {
		SelectObject(BufferHdc, hOldBitmap);
		DeleteObject(hBitmap);
		DeleteDC(BufferHdc);
	}

	bool RenderDevice::Initial(HWND hwnd,const int width,const int height) {
		{
			this->hwnd = hwnd;
			this->width = width;
			this->height = height;
		}
		{
			WindowHdc = GetDC(this->hwnd);
			BufferHdc = CreateCompatibleDC(WindowHdc);
			//ReleaseDC(this->hwnd, CurDC);
			BITMAPINFO bi =
			{
				sizeof(BITMAPINFOHEADER),
				this->width,
				this->height,
				1, 32,
				static_cast<DWORD>(BI_RGB),
				static_cast<DWORD>(width * height * sizeof(uint32_t)),
				0, 0,
				static_cast<DWORD>(0),
				static_cast<DWORD>(0)
			};

			if (!(hBitmap = CreateDIBSection(BufferHdc, &bi, DIB_RGB_COLORS, reinterpret_cast<void**>(&FrameBuffer), nullptr, 0)))
			{
				return false;
			}
			hOldBitmap = reinterpret_cast<HBITMAP>(SelectObject(BufferHdc, hBitmap));
			return true;
		}
	}

	void RenderDevice::DrawFrameBuffer() {
		//HDC hDC = GetDC(this->hwnd);
		BitBlt(WindowHdc, 0, 0, this->width, this->height, this->BufferHdc, 0, 0, SRCCOPY);
		//ReleaseDC(this->hwnd, hDC);
	}

	void RenderDevice::DrawPixel(const int x, const int y) {
		FrameBuffer[y * width + x] = 0xfffffful;
	}
}
