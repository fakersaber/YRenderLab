#include <Public/VulkanRHI/VulkanRHI.h>
#include <Public/VulkanRHI/VulkanDevice.h>




void VulkanRHI::GetInstanceLayersAndExtensions(std::vector<const char*>& Extensions, std::vector<const char*>& Layers) {

	//Extensions
	uint32_t GlobalExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &GlobalExtensionCount, nullptr);
	std::vector<VkExtensionProperties> GlobalExtensions(GlobalExtensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &GlobalExtensionCount, GlobalExtensions.data());
	std::vector<const char*> PlatformExtensions;

	VulkanPlatform::GetPlatformInstanceExtensions(PlatformExtensions);

	for (auto PlatformPtr : PlatformExtensions) {
		for (auto& GlobalStrPtr : GlobalExtensions) {
			if (!std::strcmp(PlatformPtr, GlobalStrPtr.extensionName)) {
				Extensions.emplace_back(PlatformPtr);
			}
		}
	}

	//Layers
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	std::vector<const char*> PlatformLayers;

	VulkanPlatform::GetPlatformInstanceLayers(PlatformLayers);

	for (auto PlatformPtr : PlatformLayers) {
		for (auto& availableLayerPtr : availableLayers) {
			if (!std::strcmp(PlatformPtr, availableLayerPtr.layerName)) {
				Layers.emplace_back(PlatformPtr);
			}
		}
	}

}


void VulkanDevice::GetDeviceExtensionsAndLayers(VkPhysicalDevice Gpu, std::vector<const char*>& DeviceEntensions, std::vector<const char*>& DeviceLayers) {

	//Extensions
	uint32_t GlobalExtensionCount = 0;
	vkEnumerateDeviceExtensionProperties(Gpu, nullptr, &GlobalExtensionCount, nullptr);
	std::vector<VkExtensionProperties> GlobalExtensions(GlobalExtensionCount);
	vkEnumerateDeviceExtensionProperties(Gpu, nullptr, &GlobalExtensionCount, GlobalExtensions.data());
	std::vector<const char*> PlatformDeviceExtensions;

	VulkanPlatform::GetPlatformDeviceExtensions(PlatformDeviceExtensions);

	for (auto PlatformDevicePtr : PlatformDeviceExtensions) {
		for (auto& GlobalStrPtr : GlobalExtensions) {
			if (!std::strcmp(PlatformDevicePtr, GlobalStrPtr.extensionName)) {
				DeviceEntensions.emplace_back(PlatformDevicePtr);
			}
		}
	}


	//Layers
	uint32_t layerCount;
	vkEnumerateDeviceLayerProperties(Gpu, &layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateDeviceLayerProperties(Gpu, &layerCount, availableLayers.data());
	std::vector<const char*> PlatformLayers;

	VulkanPlatform::GetPlatformDeviceLayers(PlatformLayers);

	for (auto PlatformPtr : PlatformLayers) {
		for (auto& availableLayerPtr : availableLayers) {
			if (!std::strcmp(PlatformPtr, availableLayerPtr.layerName)) {
				DeviceLayers.emplace_back(PlatformPtr);
			}
		}
	}

}