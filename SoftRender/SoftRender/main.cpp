#include <iostream>

//#include "GdiDevice.h"
#include "WindowClass.h"





int main() {

	Yrender::Window::WindowClass::GetWindowInstance(800,600)->WindowRun();

	system("pause");
}