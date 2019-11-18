#include <Public/YCore.h>

int main() {
	auto YEngine = YRender::YCore::GetCore();
	if (YEngine->Initial(1920, 1080)) {
		YEngine->Run();
	}
}