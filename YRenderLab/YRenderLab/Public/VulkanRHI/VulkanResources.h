#ifndef _YRENDER_VKRHI_VKRESOURCES_H_
#define _YRENDER_VKRHI_VKRESOURCES_H_

#include <Public/VulkanRHI/VulkanRHI.h>

class VulkanDevice;


struct VulkanTextureResource
{

	VulkanTextureResource(VulkanDevice& Device, const VkImageCreateInfo& ImageCI, VkMemoryPropertyFlagBits MemoryType);
	~VulkanTextureResource();
	void BuildTextureView(const VkImageViewCreateInfo& ImageViewCI);

	//[Resource ref]
	VkDevice DeviceRef;
	//[Resource management]
	VkImage TextureImage;
	VkImageView TextureImageView;
	VkDeviceMemory TextureMemory;
};




#endif