#ifndef _RENDERDEVICE_H
#define _RENDERDEVICE_H

#include <windows.h>
#include <iostream>



namespace YRender {
	class RenderDevice {
	public:
		RenderDevice& operator=(const RenderDevice& rhs) = delete;
		RenderDevice(const RenderDevice& rhs) = delete;
		RenderDevice();
		~RenderDevice();
		//static RenderDevice* GetDevice();

	public:
		bool Initial(HWND hwnd, const int width, const int height);
		void DrawPixel(const int x,const int y);
		void Draw();

	private:
		uint32_t* FrameBuffer;
		int width;
		int height;
		HDC Hdc;
		HBITMAP hBitmap;
		HBITMAP hOldBitmap;
		HWND hwnd;
	};
}



#endif 