#include <Public/YCore.h>


#include <Public/YGM/Transform.h>

int main() {

	auto YEngine = YRender::YCore::GetCore();
	if (YEngine->Initial(800, 600)) {
		YEngine->Run();
	}

	//YRender::YGM::Transform test = YRender::YGM::Transform::LookAt(YRender::Vector3(0.f, 10.f, -10.f),YRender::Vector3(9.f,4.f,18.072f));
	//std::cout << test.GetMatrix() << std::endl;
}