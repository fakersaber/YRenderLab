#include <Public/YCore.h>
#include <Public/VulkanRHI/VulkanRHI.h>

int main() {

	auto YEngine = YCore::GetCore();
	if (YEngine->Initial(1600, 900)) {
		YEngine->Run();
	}
	return 0;
}
