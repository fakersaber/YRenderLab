#ifndef _YRENDER_VKRHI_VulkanWindows_H_
#define _YRENDER_VKRHI_VulkanWindows_H_

#include <vulkan/vulkan.h>
#include <vector>

class VulkanWindowsPlatform {
public:
	static void GetInstanceInstanceExtensions(std::vector<const char*>& OutExtensions);

	static void GetPlatformInstanceLayers(std::vector<const char*>& OutLayers);

	static void GetPlatformDeviceExtensions(std::vector<const char*>& OutExtensions);

	static void GetPlatformDeviceLayers(std::vector<const char*>& OutLayers);

	static void CreateSurface(void* WindowHandle, VkInstance Instance, VkSurfaceKHR* OutSurface);
};


using VulkanPlatform = VulkanWindowsPlatform;

#endif