#ifndef _RENDERDEVICE_H
#define _RENDERDEVICE_H

#include <windows.h>
#include <iostream>


#include "ErrorCode.hpp"


//该类提供实际绘制功能，但不做运算，被实际窗口初始化，自己不初始化
namespace YRender {
	class RenderDevice {
	public:
		RenderDevice& operator=(const RenderDevice& rhs) = delete;
		RenderDevice(const RenderDevice& rhs) = delete;
		static RenderDevice* GetDevice();

	public:
		ErrorCode Initial(HWND hwnd, const int width, const int height);
		void DrawPixel(const int x,const int y);
		void Draw();

	private:
		RenderDevice();
		~RenderDevice();

		


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