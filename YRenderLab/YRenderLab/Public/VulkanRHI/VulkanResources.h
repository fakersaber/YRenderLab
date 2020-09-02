#ifndef _YRENDER_VKRHI_VKRESOURCES_H_
#define _YRENDER_VKRHI_VKRESOURCES_H_

#include <Public/VulkanRHI/VulkanRHI.h>
#include <Public/VulkanRHI/VulkanDevice.h>

struct VulkanTextureResource{
	VulkanTextureResource()
		: TextureImage(VK_NULL_HANDLE)
		, TextureImageView(VK_NULL_HANDLE)
		, TextureMemory(VK_NULL_HANDLE)
	{
	}

	void InitialTextureResource(
		VulkanDevice* Device,
		const VkImageCreateInfo& ImageCI, 
		VkMemoryPropertyFlagBits MemoryType,
		VkImageViewType ImageViewType,
		VkComponentMapping ComponentMapping,
		uint32_t FirstMip,
		uint32_t NumMips,
		uint32_t ArraySliceIndex,
		uint32_t NumArraySlices,
		VkImageAspectFlags AspectFlags
	);
	void ReleaseRenderResource(VulkanDevice* Device);


	//[Resource management]
	VkImage TextureImage;
	VkImageView TextureImageView;
	VkDeviceMemory TextureMemory;
};

struct VulkanBufferResource {
	VulkanBufferResource()
		: ResourceBufferMemory(VK_NULL_HANDLE)
		, ResourceBuffer(VK_NULL_HANDLE)
	{
	}

	template<bool bWriteBuffer>
	void CreateBuffer(
		VulkanDevice* Device,
		VkBufferUsageFlags BufferUsage,
		VkMemoryPropertyFlags MemoryType,
		uint32_t BufferSize,
		void* WriteBuffer
	);

	void ReleaseBuffer(VkDevice DeviceRef);

	//[Resource management]
	VkDeviceMemory ResourceBufferMemory;
	VkBuffer ResourceBuffer;
};

struct VulkanIndexBufferResource : VulkanBufferResource {
	VulkanIndexBufferResource() 
		: VulkanBufferResource()
		, IndexCount(0)
	{
	}

	template<bool bWriteBuffer>
	void CreateBuffer(
		VulkanDevice* Device,
		VkBufferUsageFlags BufferUsage,
		VkMemoryPropertyFlags MemoryType,
		uint32_t BufferSize,
		void* WriteBuffer
	);

	uint32_t IndexCount;
};


//Use AOS
struct VulkanVertexBufferResource : VulkanBufferResource{
	VulkanVertexBufferResource()
		: VulkanBufferResource()
	{
	}
};

#include <Public/VulkanRHI/VulkanResources.inl>


#endif