#include <Public/YCore.h>

#include <Public/Scene/AssimpLoader.h>



int main() {

	std::shared_ptr<int> testptr = std::make_shared<int>(3);

	auto Scene = YRender::AssimpLoader::Load("C:/Users/Administrator/Desktop/nanosuit/nanosuit.obj");
	std::cout << Scene.use_count() << std::endl;






	//auto YEngine = YRender::YCore::GetCore();
	//if (YEngine->Initial(800, 600)) {
	//	YEngine->Run();
	//}
}