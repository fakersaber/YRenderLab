#include <Public/YCore.h>


class TestClass {
public:
	TestClass(int param) : test(param) {}

	int test = 0;
};



std::shared_ptr<TestClass> fun1() {
	auto Rtl = std::make_shared<TestClass>(2);
	return Rtl;
}



int main() {
	auto YEngine = YRender::YCore::GetCore();
	if (YEngine->Initial(1920, 1080)) {
		YEngine->Run();
	}
}