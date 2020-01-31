#include <Public/YCore.h>

#include <iostream>
#include <unordered_set>

#include <Public/Basic/File/File.h>


int main() {
	//auto test = File::ReadAllLines("Data/../Data/shaders/P3N3T2T3.vs");
	auto YEngine = YCore::GetCore();
	if (YEngine->Initial(800, 600)) {
		YEngine->Run();
	}
	return 0;
}