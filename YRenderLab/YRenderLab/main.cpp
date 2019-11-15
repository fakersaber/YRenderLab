#include <Public/YCore.h>

#include <Public/Scene/AssimpLoader.h>
#include <Public/Scene/Scene.h>
#include <map>

int main() {

	//std::shared_ptr<int> ptr1(new int(1));
	//std::weak_ptr<int> wptr = ptr1;
	//testmap[wptr] = 2;

	//testmap.find(ptr1);

	auto Root = YRender::AssimpLoader::Load("C:/Users/Administrator/Desktop/nanosuit/nanosuit.obj");
	auto Scene = YRender::New<YRender::Scene>(Root);

	//auto YEngine = YRender::YCore::GetCore();
	//if (YEngine->Initial(800, 600)) {
	//	YEngine->Run();
	//}
}