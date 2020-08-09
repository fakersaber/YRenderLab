#include <Public/YCore.h>
#include <vector>


int main() {
	auto YEngine = YCore::GetCore();

	YEngine->Init(800, 600);

	YEngine->Tick();

	YEngine->Shutdown();

	return 0;
}
