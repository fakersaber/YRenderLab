#include <public/SoftRHI/GdiDevice.h>

namespace YRender {

	//RenderDevice* RenderDevice::GetDevice() {
	//	static RenderDevice instance;
	//	return &instance;
	//}

	GdiDevice::GdiDevice(HWND hwnd) :
		BufferHdc(nullptr),
		hBitmap(nullptr),
		hOldBitmap(nullptr),
		hwnd(hwnd){ }

	GdiDevice::~GdiDevice() {
		SelectObject(BufferHdc, hOldBitmap);
		DeleteObject(hBitmap);
		DeleteDC(BufferHdc);
	}

	bool GdiDevice::Initial(const int width, const int height) {
		this->width = width;
		this->height = height;
		WindowHdc = GetDC(hwnd);
		BufferHdc = CreateCompatibleDC(WindowHdc);

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

	void GdiDevice::DrawFrameBuffer() {
		BitBlt(WindowHdc, 0, 0, this->width, this->height, this->BufferHdc, 0, 0, SRCCOPY);
	}

	void GdiDevice::DrawPixel(const int x, const int y) {
		FrameBuffer[y * width + x] = 0xfffffful;
	}

	void GdiDevice::DrawPixel(const int x, const int y, const RGBAf& color) {
		FrameBuffer[y * width + x] = color.RBGA2UINT();
	}
}
