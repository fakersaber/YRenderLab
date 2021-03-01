#include <Public/GlfwApplication.h>
#include <memory>
#include <iostream>



int main() {
	auto YApplication = std::make_unique<GlfwApplication>();
	YApplication->Init(800, 600);
	YApplication->Tick();
	YApplication->Shutdown();
	return 0;
}
