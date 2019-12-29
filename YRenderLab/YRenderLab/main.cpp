#include <Public/YCore.h>


class TestClass2 {
//public:
//	TestClass2() = default;
private:
	double test2 = 0.0;
};


class TestClass :public TestClass2 {
public:
	TestClass() = default;
	TestClass(int param) : test(param){}

	int test = 0;
};



std::shared_ptr<TestClass> fun1() {
	auto Rtl = std::make_shared<TestClass>(2);
	return Rtl;
}



int main() {
	auto YEngine = YRender::YCore::GetCore();
	if (YEngine->Initial(800, 600)) {
		YEngine->Run();
	}
}