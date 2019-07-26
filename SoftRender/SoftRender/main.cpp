#include <iostream>

#include "WindowClass.h"

#include "ErrorCode.hpp"



int main() {

	auto WindowInstan = YRender::WindowClass::GetWindow();

	if (WindowInstan->WindowInit(800, 600)==ErrorCode::INIT_SUCCESS) {
		WindowInstan->WindowRun();
	}
}