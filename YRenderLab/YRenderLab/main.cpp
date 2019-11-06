#include <Public/YCore.h>


#include <Public/Basic/StrApi.h>

//class A : public YRender::YHeapObject {
//public:
//	static std::shared_ptr<A> New(int a) { return YRender::New<A>(a); }
//public:
//	A() {}
//	virtual ~A() = default;
//
//	void TestCast() {
//		auto a = shared_this<A>();
//		auto b = weak_this<A>();
//	}
//private:
//	int a;
//};



//class ObjectTest :public YRender::YHeapObject{
//public:
//	ObjectTest() = default;
//	virtual ~ObjectTest() = default;
//};

//std::shared_ptr<A> ptr = A::New(1);
//std::shared_ptr<ObjectTest> p = YRender::Cast<ObjectTest>(ptr);
//std::cout << ptr.use_count() << std::endl;
//auto ptr = YRender::New<ObjectTest>();
//std::cout << ptr.use_count() << std::endl;


int main() {
	


	auto YEngine = YRender::YCore::GetCore();
	if (YEngine->Initial(800, 600)) {
		YEngine->Run();
	}
}