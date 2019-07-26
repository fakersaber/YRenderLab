#include <iostream>

#include "YCore.h"



int main() {

	auto YEngine = YRender::YCore::GetCore();
	if (YEngine->Initial(800, 600)) {
		YEngine->Run();
	}
}