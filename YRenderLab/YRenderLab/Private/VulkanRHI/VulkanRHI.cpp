#include <Public/VulkanRHI/VulkanRHI.h>
#include <vector>

VulkanRHI::VulkanRHI()
	:Instance(VK_NULL_HANDLE)
	, Devices(nullptr)
{

}

void VulkanRHI::Init(){
	CreateInstance();
}

void VulkanRHI::CreateInstance(){
	VkApplicationInfo AppInfo = {};
	AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	AppInfo.pApplicationName = "YRenderLab";
	AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	AppInfo.pEngineName = "YRenderLab";
	AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	AppInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo InstInfo = {};
	InstInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InstInfo.pApplicationInfo = &AppInfo;


	GetInstanceExtensionsAndLayers(InstanceExtensions, InstanceLayers);

}

void VulkanRHI::GetInstanceExtensionsAndLayers(std::vector<const char*>& Entensions, std::vector<const char*>& Layers){

	//GLFW
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = nullptr;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	
	Entensions.reserve(glfwExtensionCount);
	for (auto i = 0; i < glfwExtensionCount; ++i) {
		Entensions.emplace_back(glfwExtensions[i]);
	}
}
