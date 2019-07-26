#ifndef _WINDOWTOOL_H
#define _WINDOWTOOL_H

#include <windows.h>
#include <iostream>



namespace YRender {

	class RenderWindow {
	public:
		RenderWindow(const RenderWindow& rhs) = delete;
		RenderWindow& operator=(const RenderWindow& rhs) = delete;
		static RenderWindow* GetWindow();

	public:
		bool Initial(const int width, const int height);
		
		inline HWND GetHwnd() {
			return this->hwnd;
		}
	private:
		RenderWindow();
		~RenderWindow();


	private:
		static LRESULT WINAPI WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HWND hwnd;
	};
}


#endif
