#include "CreateWindow.h"



namespace Window {

	RenderWindow* RenderWindow::GetWindow() {
		static RenderWindow* instance = new RenderWindow();

	}
}