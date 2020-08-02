#ifndef _YRENDER_VKRHI_VKQUEUE_H_
#define _YRENDER_VKRHI_VKQUEUE_H_

#include <iostream>
#include <vulkan/vulkan.h>

class VulkanDevice;


class VulkanQueue {
public:

	VulkanQueue(VulkanDevice* InDevice, uint32_t InFamilyIndex);
	~VulkanQueue();


	inline VkQueue GetHandle() const
	{
		return Queue;
	}

	inline uint32_t GetFamilyIndex() const
	{
		return FamilyIndex;
	}

private:
	VkQueue Queue;
	VulkanDevice* Device;
	uint32_t FamilyIndex;
	uint32_t QueueIndex;
};





#endif