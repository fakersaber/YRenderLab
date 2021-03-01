#include <Public/VulkanRHI/VulkanWindowsPlatform.h>
#include <Public/VulkanRHI/VulkanRHI.h>

//#ifdef VK_USE_PLATFORM_WIN32_KHR
#include <windows.h>
#include <vulkan/vulkan_win32.h>
//#endif

//STANDARD_VALIDATION_LAYER_NAME是即将被抛弃的版本
#if VULKAN_HAS_DEBUGGING_ENABLED
#define KHRONOS_STANDARD_VALIDATION_LAYER_NAME	"VK_LAYER_KHRONOS_validation"
#define RENDERDOC_LAYER_NAME				"VK_LAYER_RENDERDOC_Capture"
#endif


void VulkanWindowsPlatform::GetPlatformInstanceExtensions(std::vector<const char*>& OutExtensions){

	OutExtensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME); //VK_KHR_surface
	OutExtensions.emplace_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

#if VULKAN_HAS_DEBUGGING_ENABLED
	OutExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

//#if VULKAN_EXT_SWAPCHIAN_COLORSPACE
//	OutExtensions.emplace_back(VK_EXT_SWAPCHAIN_COLOR_SPACE_EXTENSION_NAME);
//#endif
}


void VulkanWindowsPlatform::GetPlatformInstanceLayers(std::vector<const char*>& OutLayers) {
#if VULKAN_HAS_DEBUGGING_ENABLED
	OutLayers.emplace_back(KHRONOS_STANDARD_VALIDATION_LAYER_NAME);
#endif
}

void VulkanWindowsPlatform::GetPlatformDeviceExtensions(std::vector<const char*>& OutExtensions){
	OutExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

void VulkanWindowsPlatform::GetPlatformDeviceLayers(std::vector<const char*>& OutLayers){

}

void VulkanWindowsPlatform::CreateSurface(void* WindowHandle, VkInstance Instance, VkSurfaceKHR* OutSurface){
	VkWin32SurfaceCreateInfoKHR SurfaceCreateInfo = {};
	SurfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	SurfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
	SurfaceCreateInfo.hwnd = reinterpret_cast<HWND>(WindowHandle);
	auto Result = vkCreateWin32SurfaceKHR(Instance, &SurfaceCreateInfo, nullptr, OutSurface);
	assert(Result == VK_SUCCESS);
	if (Result != VK_SUCCESS) {
		std::cerr << "Surface Create Failed !" << std::endl;
	}
}
