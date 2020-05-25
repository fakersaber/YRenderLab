#ifndef _YRENDER_VKRHI_VKDevice_H_
#define _YRENDER_VKRHI_VKDevice_H_

#include <vulkan/vulkan.h>

class VulkanRHI;

class YVulkanDevice {

	enum class VenderID : uint32_t {
		Nvidia = 0x10DE,
		AMD = 0x1002
	};

public:
	YVulkanDevice(VulkanRHI* InRHI, VkPhysicalDevice InGpu);

	~YVulkanDevice();

	// Returns true if this is a viable candidate for main GPU
	bool QueryGPU();

private:
	VulkanRHI* VkRHI;

	VkDevice Device;

	VkPhysicalDevice Gpu;
};


#endif