#include <Public/VulkanRHI/VulkanResources.h>
#include <Public/VulkanRHI/VulkanDevice.h>
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


template<bool bWriteData>
void VulkanIndexBufferResource::InitialIndexBufferResource(
	VulkanDevice* Device,
	VkBufferUsageFlags BufferUsage,
	VkMemoryPropertyFlags MemortType,
	uint32_t InIndexSize,
	const std::vector<uint32_t>& InIndexBufferData
){
	auto DeviceRef = Device->GetLogicDevice();
	IndexCount = InIndexSize;
	VkDeviceSize IndexBufferSize = InIndexSize * sizeof(uint32_t);
	VkMemoryAllocateInfo memAlloc = {};
	memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	VkMemoryRequirements memReqs;
	VkBufferCreateInfo indexbufferInfo = {};
	indexbufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	indexbufferInfo.size = IndexBufferSize;
	indexbufferInfo.usage = BufferUsage;

	//Create Memory
	assert(vkCreateBuffer(DeviceRef, &indexbufferInfo, nullptr, &IndexBuffer) == VK_SUCCESS);
	vkGetBufferMemoryRequirements(DeviceRef, IndexBuffer, &memReqs);
	memAlloc.allocationSize = memReqs.size;
	memAlloc.memoryTypeIndex = Device->GetMemoryTypeIndex(memReqs.memoryTypeBits, MemortType);
	assert(vkAllocateMemory(DeviceRef, &memAlloc, nullptr, &IndexBufferMemory) == VK_SUCCESS);

	//Write data and bind
	if (bWriteData) {
		void* MappingMemory = nullptr;
		assert(vkMapMemory(DeviceRef, IndexBufferMemory, 0, IndexBufferSize, 0, &MappingMemory) == VK_SUCCESS);
		std::memcpy(MappingMemory, InIndexBufferData.data(), IndexBufferSize);
		vkUnmapMemory(DeviceRef, IndexBufferMemory);
	}

	assert(vkBindBufferMemory(DeviceRef, IndexBuffer, IndexBufferMemory, 0) == VK_SUCCESS);
}

void VulkanIndexBufferResource::ReleaseRenderResource(VulkanDevice* Device){
	vkDestroyBuffer(Device->GetLogicDevice(), IndexBuffer, nullptr);
	vkFreeMemory(Device->GetLogicDevice(), IndexBufferMemory, nullptr);
}

void VulkanVertexBufferResource::ReleaseRenderResource(VulkanDevice* Device){
	vkDestroyBuffer(Device->GetLogicDevice(), VertexBuffer, nullptr);
	vkFreeMemory(Device->GetLogicDevice(), VertexBufferMemory, nullptr);
}
