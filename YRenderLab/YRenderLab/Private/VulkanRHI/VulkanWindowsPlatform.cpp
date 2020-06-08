#include <Public/VulkanRHI/VulkanWindowsPlatform.h>
#include <Public/VulkanRHI/VulkanRHI.h>
#include <windows.h>
#include <vulkan/vulkan_win32.h>


//STANDARD_VALIDATION_LAYER_NAME是即将被抛弃的版本
#if VULKAN_HAS_DEBUGGING_ENABLED
#define KHRONOS_STANDARD_VALIDATION_LAYER_NAME	"VK_LAYER_KHRONOS_validation"
#define RENDERDOC_LAYER_NAME				"VK_LAYER_RENDERDOC_Capture"
#endif


void VulkanWindowsPlatform::GetInstanceInstanceExtensions(std::vector<const char*>& OutExtensions){

	OutExtensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME); //VK_KHR_surface
	OutExtensions.emplace_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

#if VULKAN_SUPPORTS_DEBUG_UTILS
	OutExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
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
