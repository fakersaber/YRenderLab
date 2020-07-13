#include <Public/YCore.h>
#include <vector>


int main() {
	auto YEngine = YCore::GetCore();

	YEngine->Init(1600, 900);

	YEngine->Tick();

	YEngine->Shutdown();

	return 0;
}
