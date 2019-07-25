#ifndef _GDIDEVICE_H
#define _GDIDEVICE_H

#include <windows.h>


namespace YRender{
	//Render类只处理GDI设备
	class GdiDevice{
	public:	
		GdiDevice(const GdiDevice& rhs) = delete;
		GdiDevice& operator=(const GdiDevice& rhs) = delete;
		static GdiDevice* GetRenderInstance();

	private:
		GdiDevice();
		~GdiDevice();

	private:
		HDC Hdc;
		HBITMAP hBitmap;
		HBITMAP hOldBitmap;
	};
}


#endif
