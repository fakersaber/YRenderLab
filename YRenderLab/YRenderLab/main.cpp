#include <Public/YCore.h>


int main() {
	//auto test = File::ReadAllLines("Data/../Data/shaders/P3N3T2T3.vs");
	auto YEngine = YCore::GetCore();
	if (YEngine->Initial(1600, 900)) {
		YEngine->Run();
	}
	return 0;
}