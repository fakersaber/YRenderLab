#include <Public/VulkanRHI/VulkanResources.h>
#include <Public/VulkanRHI/VulkanDevice.h>



VulkanTextureResource::VulkanTextureResource(VulkanDevice& Device, const VkImageCreateInfo& ImageCI, VkMemoryPropertyFlagBits MemoryType)
	: DeviceRef(Device.GetLogicDevice())
	, TextureImageView(nullptr)
{
	//Image
	assert(vkCreateImage(DeviceRef, &ImageCI, nullptr, &TextureImage) == VK_SUCCESS);

	//Memory Allocate and bind
	VkMemoryRequirements memReqs{};
	vkGetImageMemoryRequirements(DeviceRef, TextureImage, &memReqs);
	VkMemoryAllocateInfo memAllloc{};
	memAllloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAllloc.allocationSize = memReqs.size;
	memAllloc.memoryTypeIndex = Device.GetMemoryTypeIndex(memReqs.memoryTypeBits, MemoryType);
	assert(vkAllocateMemory(DeviceRef, &memAllloc, nullptr, &TextureMemory) == VK_SUCCESS);
	assert(vkBindImageMemory(DeviceRef, TextureImage, TextureMemory, 0) == VK_SUCCESS);

}


VulkanTextureResource::~VulkanTextureResource(){
	if (TextureImageView) {
		vkDestroyImageView(DeviceRef, TextureImageView, nullptr);
	}
	vkDestroyImage(DeviceRef, TextureImage, nullptr);
	vkFreeMemory(DeviceRef, TextureMemory, nullptr);
}


void VulkanTextureResource::BuildTextureView(const VkImageViewCreateInfo& ImageViewCI){
	if (TextureImageView) {
		vkDestroyImageView(DeviceRef, TextureImageView, nullptr);
	}
	assert(vkCreateImageView(DeviceRef, &ImageViewCI, nullptr, &TextureImageView) == VK_SUCCESS);
}


