#include <Public/YCore.h>


#include <Public/Basic/StrApi.h>

#include <Public/Scene/YObject.h>
#include <Public/Scene/TransformComponent.h>

class A {
public:
	A() = default;
private:
	~A() = default;
};


int main() {
	auto obj = YRender::New<YRender::YObject>("test");
	std::shared_ptr<YRender::Component> ComponentPtr = YRender::New<YRender::TransformComponent>(obj);
	auto& Components = obj->Getcomponents();
	obj->DetachComponent(ComponentPtr);
	obj->AttachComponent(ComponentPtr);
	if (Components.find(ComponentPtr) != Components.end()) {
		std::cout << ComponentPtr.use_count() << std::endl;
	}
	
	//auto YEngine = YRender::YCore::GetCore();
	//if (YEngine->Initial(800, 600)) {
	//	YEngine->Run();
	//}
}