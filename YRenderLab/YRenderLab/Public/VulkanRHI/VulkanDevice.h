#ifndef _YRENDER_VKRHI_VKDevice_H_
#define _YRENDER_VKRHI_VKDevice_H_

#include <vulkan/vulkan.h>
#include <vector>

class VulkanRHI;
class VulkanQueue;

class VulkanDevice {

public:
	VulkanDevice(VulkanRHI* InRHI, VkPhysicalDevice InGpu);

	~VulkanDevice();

	// Returns true if this is a viable candidate for main GPU
	bool QueryGPU();

	void InitGPU();

	void CreateDevice();

	//This function is used to bind presentQueue
	void SetupPresentQueue(VkSurfaceKHR Surface);

	static void GetDeviceExtensionsAndLayers(VkPhysicalDevice Gpu, std::vector<const char*>& Entensions, std::vector<const char*>& Layers);

	inline VkPhysicalDevice GetPhysicalDevice() const{
		return PhysicalDevice;
	}

	inline VkDevice GetLogicDevice() const {
		return LogicalDevice;
	}

	inline VulkanRHI* GetVulkanRHI() const {
		return VkRHI;
	}

private:
	VulkanRHI* VkRHI;
	VkDevice LogicalDevice;
	VkPhysicalDevice PhysicalDevice;
	VkPhysicalDeviceFeatures PhysicalFeatures;

	std::vector<VkQueueFamilyProperties> QueueFamilyProps;
	std::vector<const char*> DeviceExtensions;
	std::vector<const char*> DeviceLayers;

	VulkanQueue* GfxQueue;
	VulkanQueue* ComputeQueue;
	VulkanQueue* TransferQueue;
	VulkanQueue* PresentQueue;

};


#endif