#include <Public/RenderDevice.h>



namespace YRender{
	//RenderDevice* RenderDevice::GetDevice() {
	//	static RenderDevice instance;
	//	return &instance;
	//}

	RenderDevice::RenderDevice():
		Hdc(nullptr), 
		hBitmap(nullptr), 
		hOldBitmap(nullptr)
	{

	}

	RenderDevice::~RenderDevice() {
		SelectObject(Hdc, hOldBitmap);
		DeleteObject(hBitmap);
		DeleteDC(Hdc);
	}

	bool RenderDevice::Initial(HWND hwnd,const int width,const int height) {
		{
			this->hwnd = hwnd;
			this->width = width;
			this->height = height;
		}
		{
			HDC CurDC = GetDC(this->hwnd);
			Hdc = CreateCompatibleDC(CurDC);
			ReleaseDC(this->hwnd, CurDC);
			BITMAPINFO bi =
			{
				sizeof(BITMAPINFOHEADER),
				this->width,
				-this->height,
				1, 32,
				static_cast<DWORD>(BI_RGB),
				static_cast<DWORD>(width * height * sizeof(uint32_t)),
				0, 0,
				static_cast<DWORD>(0),
				static_cast<DWORD>(0)
			};

			if (!(hBitmap = CreateDIBSection(Hdc, &bi, DIB_RGB_COLORS, reinterpret_cast<void**>(&FrameBuffer), nullptr, 0)))
			{
				return false;
			}
			hOldBitmap = reinterpret_cast<HBITMAP>(SelectObject(Hdc, hBitmap));
			return true;
		}
	}

	void RenderDevice::DrawFrameBuffer() {
		HDC hDC = GetDC(this->hwnd);
		BitBlt(hDC, 0, 0, this->width, this->height, this->Hdc, 0, 0, SRCCOPY);
		ReleaseDC(this->hwnd, hDC);
	}

	void RenderDevice::DrawPixel(const int x, const int y) {
		FrameBuffer[y * width + x] = 0xfffffful;
	}
}
