#include "GdiDevice.h"
#include "WindowTool.h"


#include <iostream>
/*
If biHeight is positive, the bitmap is a bottom - up DIB and its origin is the lower - left corner.
If biHeight is negative, the bitmap is a top - down DIB and its origin is the upper - left corner.
If biHeight is negative, indicating a top - down DIB, biCompression must be either BI_RGB or BI_BITFIELDS.
*/

namespace YRender {
	GdiDevice::GdiDevice() {
		//HDC CurDC = GetDC(Window::hwnd);
		//this->Hdc = CreateCompatibleDC(CurDC);
		//::ReleaseDC(Window::hwnd,CurDC);
		//BITMAPINFO bi = { 
		//	sizeof(BITMAPINFOHEADER), Window::WindowWidth, -Window::WindowHeight, 
		//	1, 32, BI_RGB,Window::WindowWidth * Window::WindowHeight * 4,0, 0, 0, 0 
		//};
		//if (CreateDIBSection(this->Hdc, &bi, DIB_RGB_COLORS, , nullptr, 0) == nullptr) 
		//{
		//	std::cout << "Create DIB failed" << std::endl;
		//	return;
		//}

		//hOldBitmap = reinterpret_cast<HBITMAP>(SelectObject(this->Hdc, hBitmap));
	}


	GdiDevice* GdiDevice::GetRenderInstance() {
		static GdiDevice* instance = new GdiDevice();
		return instance;
	}
}