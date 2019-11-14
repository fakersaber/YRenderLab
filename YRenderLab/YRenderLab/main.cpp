#include <Public/YCore.h>

#include <Public/Scene/AssimpLoader.h>
#include <Public/Scene/Scene.h>


int main() {
	auto Root = YRender::AssimpLoader::Load("C:/Users/Administrator/Desktop/nanosuit/nanosuit.obj");
	auto Scene = YRender::New<YRender::Scene>(Root);

	//auto YEngine = YRender::YCore::GetCore();
	//if (YEngine->Initial(800, 600)) {
	//	YEngine->Run();
	//}
}