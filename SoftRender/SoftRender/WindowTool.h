#ifndef _WINDOWTOOL_H
#define _WINDOWTOOL_H

#include <windows.h>

namespace Window {
	extern HWND hwnd;
	extern int WindowWidth;
	extern int WindowHeight;

	LRESULT WINAPI WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void WindowInit(const int& width, const int& height);
}


#endif
