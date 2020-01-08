#ifndef _YRENDER_RENDERDEVICE_H
#define _YRENDER_RENDERDEVICE_H

#include <iostream>
#include <Public/YGM/RGBA.hpp>



	class RenderDevice {
	public:
		RenderDevice& operator=(const RenderDevice& rhs) = delete;
		RenderDevice(const RenderDevice& rhs) = delete;

	public:
		virtual bool Initial(const int width, const int height) = 0;
		virtual const int GetHeight() const = 0;
		virtual const int GetWidth() const = 0;
		virtual void DrawFrameBuffer() = 0;
		virtual void ClearFrameBuffer() = 0;
		virtual void DrawPixel(const int x, const int y, const RGBAf& color = RGBAf(0.f)) = 0;

	public:
		RenderDevice();
		virtual ~RenderDevice();

	protected:
		int32_t width;
		int32_t height;
	};




#endif 