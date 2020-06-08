#include <Public/YCore.h>
#include <vector>


int main() {

	std::vector<int> foo;

	foo.resize(10);

	foo.resize(11);

	auto YEngine = YCore::GetCore();
	if (YEngine->Initial(1600, 900)) {
		YEngine->Run();
		YEngine->Shutdown();
	}
	return 0;
}
