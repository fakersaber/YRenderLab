#include <Public/VulkanRHI/VulkanResources.h>
#include <memory>



void VulkanTextureResource::InitialTextureResource(
	VulkanDevice* Device,
	const VkImageCreateInfo& ImageCI,
	VkMemoryPropertyFlagBits MemoryType,
	VkImageViewType ImageViewType,
	VkComponentMapping ComponentMapping,
	uint32_t FirstMip,
	uint32_t NumMips,
	uint32_t ArraySliceIndex,
	uint32_t NumArraySlices,
	VkImageAspectFlags AspectFlags) {
	//Image
	auto DeviceRef = Device->GetLogicDevice();
	assert(vkCreateImage(DeviceRef, &ImageCI, nullptr, &TextureImage) == VK_SUCCESS);

	//Memory Allocate and bind
	VkMemoryRequirements memReqs{};
	vkGetImageMemoryRequirements(DeviceRef, TextureImage, &memReqs);
	VkMemoryAllocateInfo memAllloc{};
	memAllloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAllloc.allocationSize = memReqs.size;
	memAllloc.memoryTypeIndex = Device->GetMemoryTypeIndex(memReqs.memoryTypeBits, MemoryType);
	assert(vkAllocateMemory(DeviceRef, &memAllloc, nullptr, &TextureMemory) == VK_SUCCESS);
	assert(vkBindImageMemory(DeviceRef, TextureImage, TextureMemory, 0) == VK_SUCCESS);

	VkImageViewCreateInfo imageViewCI{};
	imageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCI.viewType = ImageViewType;
	imageViewCI.image = TextureImage;
	imageViewCI.format = ImageCI.format;
	imageViewCI.components = ComponentMapping;
	imageViewCI.subresourceRange.baseMipLevel = FirstMip;
	imageViewCI.subresourceRange.levelCount = NumMips;
	imageViewCI.subresourceRange.baseArrayLayer = ArraySliceIndex;
	imageViewCI.subresourceRange.layerCount = NumArraySlices;
	imageViewCI.subresourceRange.aspectMask = AspectFlags;
	assert(vkCreateImageView(DeviceRef, &imageViewCI, nullptr, &TextureImageView) == VK_SUCCESS);
}

void VulkanTextureResource::ReleaseRenderResource(VulkanDevice* Device){
	vkDestroyImageView(Device->GetLogicDevice(), TextureImageView, nullptr);
	vkDestroyImage(Device->GetLogicDevice(), TextureImage, nullptr);
	vkFreeMemory(Device->GetLogicDevice(), TextureMemory, nullptr);
}


void VulkanBufferResource::ReleaseBuffer(VkDevice DeviceRef){
	vkDestroyBuffer(DeviceRef, ResourceBuffer, nullptr);
	vkFreeMemory(DeviceRef, ResourceBufferMemory, nullptr);
}

void VulkanUniformBufferResource::UpdateBuffer(VkDevice DeviceRef, void* UpdateData){
	void* UniformData = nullptr;
	assert(vkMapMemory(DeviceRef, ResourceBufferMemory, 0, Descriptor.range, 0, &UniformData) == VK_SUCCESS);
	std::memcpy(UpdateData, UniformData, Descriptor.range);
	vkUnmapMemory(DeviceRef, ResourceBufferMemory);
}
