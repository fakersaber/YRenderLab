#ifndef _YRENDER_RENDERDEVICE_H
#define _YRENDER_RENDERDEVICE_H

#include <iostream>



namespace YRender {
	class RenderDevice {
	public:
		RenderDevice& operator=(const RenderDevice& rhs) = delete;
		RenderDevice(const RenderDevice& rhs) = delete;

	public:
		virtual bool Initial(const int width, const int height) = 0;
		virtual const int GetHeight() const = 0;
		virtual const int GetWidth() const = 0;
		virtual void DrawFrameBuffer() = 0;
		virtual void DrawPixel(const int x, const int y) = 0;

	public:
		RenderDevice();
		virtual ~RenderDevice();

	protected:
		int height;
		int width;
	};
}



#endif 