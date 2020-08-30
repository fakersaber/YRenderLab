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
	VulkanQueue* SetupPresentQueue(VkSurfaceKHR Surface);
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

	inline uint32_t GetMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties) const {
		for (uint32_t i = 0; i < MemoryProperties.memoryTypeCount; i++) {
			if (typeBits & 0x1ul && (MemoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
			typeBits >>= 1ul;
		}
		throw std::runtime_error("Could not find a matching memory type");
	}

private:
	//[Resource ref]
	VulkanRHI* VkRHI;

	//[Resource management]
	VkDevice LogicalDevice;
	VkPhysicalDevice PhysicalDevice;
	VkPhysicalDeviceFeatures PhysicalFeatures;
	std::vector<VkQueueFamilyProperties> QueueFamilyProps;
	VkPhysicalDeviceMemoryProperties MemoryProperties;
	std::vector<const char*> DeviceExtensions;
	std::vector<const char*> DeviceLayers;
	VulkanQueue* GfxQueue;
	VulkanQueue* ComputeQueue;
	VulkanQueue* TransferQueue;
};


#endif