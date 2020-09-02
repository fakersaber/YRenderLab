#ifndef _YRENDER_VKRHI_VKQUEUE_H_
#define _YRENDER_VKRHI_VKQUEUE_H_

#include <iostream>
#include <vulkan/vulkan.h>

class VulkanDevice;


class VulkanQueue {
public:
	VulkanQueue(VulkanDevice* InDevice, uint32_t InFamilyIndex);
	~VulkanQueue();

	inline VkQueue GetQueue() const{
		return Queue;
	}

	inline uint32_t GetFamilyIndex() const{
		return FamilyIndex;
	}

private:
	//[Resource ref]
	VulkanDevice* Device;

	//[Resource management]
	VkQueue Queue;
	uint32_t FamilyIndex;
	uint32_t QueueIndex;
};


#endif