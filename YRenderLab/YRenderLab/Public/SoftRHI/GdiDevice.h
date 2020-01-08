#ifndef _YRENDER_SOFTRHI_GDIDEVICE_H
#define _YRENDER_SOFTRHI_GDIDEVICE_H

#include <Public/RenderDevice.h>
#include <Public/Basic/Image/Image.h>

#include <windows.h>


	class GdiDevice : public RenderDevice
	{
	public:
		GdiDevice& operator=(const GdiDevice& rhs) = delete;
		GdiDevice(const GdiDevice& rhs) = delete;
		GdiDevice() = delete;
		explicit GdiDevice(HWND hwnd);
		virtual ~GdiDevice();

	public:
		virtual bool Initial(const int width, const int height) override;
		virtual void DrawPixel(const int x, const int y, const RGBAf& color = RGBAf(0.f)) override;
		virtual void DrawFrameBuffer() override;
		virtual void ClearFrameBuffer() override;
		virtual const int GetWidth() const override { return this->width; }
		virtual const int GetHeight() const override { return this->height; }

	private:
		HWND hwnd;
		uint32_t* FrameBuffer;
		HDC BufferHdc;
		HDC WindowHdc;
		HBITMAP hBitmap;
		HBITMAP hOldBitmap;
	};






#endif