#include <Public/YCore.h>
#include <vector>


int main() {

	auto YEngine = YCore::GetCore();
	if (YEngine->Initial(1600, 900)) {
		YEngine->Run();
		YEngine->Shutdown();
	}
	return 0;
}
