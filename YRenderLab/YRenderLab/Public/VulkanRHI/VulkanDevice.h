#ifndef _YRENDER_VKRHI_VKDevice_H_
#define _YRENDER_VKRHI_VKDevice_H_

#include <vulkan/vulkan.h>
#include <vector>

class VulkanRHI;

class VulkanDevice {

public:
	VulkanDevice(VulkanRHI* InRHI, VkPhysicalDevice InGpu);

	~VulkanDevice();

	// Returns true if this is a viable candidate for main GPU
	bool QueryGPU();

	void InitGPU();

	void CreateDevice();

	static void GetDeviceExtensionsAndLayers(VkPhysicalDevice Gpu, std::vector<const char*>& Entensions, std::vector<const char*>& Layers);

	inline VkPhysicalDevice GetPhysicalHandle() const
	{
		return Gpu;
	}

private:
	VulkanRHI* VkRHI;

	VkDevice Device;

	VkPhysicalDevice Gpu;

	VkPhysicalDeviceFeatures PhysicalFeatures;

	std::vector<VkQueueFamilyProperties> QueueFamilyProps;

	std::vector<const char*> DeviceExtensions;
	std::vector<const char*> DeviceLayers;

	VkQueue GfxQueue;
	VkQueue ComputeQueue;
	VkQueue TransferQueue;


	//SwapChain
	VkSurfaceKHR Surface;

};


#endif