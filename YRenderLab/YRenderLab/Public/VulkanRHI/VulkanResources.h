#ifndef _YRENDER_VKRHI_VKRESOURCES_H_
#define _YRENDER_VKRHI_VKRESOURCES_H_

#include <Public/VulkanRHI/VulkanRHI.h>

class VulkanDevice;

struct RenderResource {

};

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

struct VulkanIndexBufferResource {
	VulkanIndexBufferResource() 
		: IndexBufferMemory(VK_NULL_HANDLE)
		, IndexBuffer(VK_NULL_HANDLE)
		, IndexCount(0)
	{
	}

	template<bool bWriteData>
	void InitialIndexBufferResource(
		VulkanDevice* Device,
		VkBufferUsageFlags BufferUsage,
		VkMemoryPropertyFlags MemortType,
		uint32_t InIndexSize,
		const std::vector<uint32_t>& InIndexBufferData);



	void ReleaseRenderResource(VulkanDevice* Device);

	//[Resource management]
	VkDeviceMemory IndexBufferMemory;
	VkBuffer IndexBuffer;
	uint32_t IndexCount;
};


//Use AOS
struct VulkanVertexBufferResource {
	VulkanVertexBufferResource()
		: VertexBufferMemory(VK_NULL_HANDLE)
		, VertexBuffer(VK_NULL_HANDLE)
	{
	}

	void ReleaseRenderResource(VulkanDevice* Device);

	VkDeviceMemory VertexBufferMemory;
	VkBuffer VertexBuffer;
};


#endif