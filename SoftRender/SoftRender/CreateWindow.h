#ifndef _CREATEWINDOW_H
#define _CREATEWINDOW_H

#include <windows.h>


namespace Window {
	class RenderWindow {
	public:	
		RenderWindow(const RenderWindow& rhs) = delete;
		RenderWindow& operator=(const RenderWindow& rhs) = delete;
		~RenderWindow();
		static RenderWindow* GetWindow();
		void Initialize();


	private:
		RenderWindow();

	};
}


#endif
