#ifndef _YRENDER_VKRHI_VKRESOURCES_H_
#define _YRENDER_VKRHI_VKRESOURCES_H_

#include <Public/VulkanRHI/VulkanRHI.h>

class VulkanDevice;


//it’s not quite ready to be used as a render target just yet.That requires one more
//step of indirection, known as a framebuffer.But first we’ll have to set up the
//graphics pipeline.

class VulkanTextureView
{
public:

	VulkanTextureView(VulkanDevice& Device,
		VkImage InImage,
		VkImageViewType ViewType,
		VkImageAspectFlags AspectFlags,
		VkComponentMapping ComponentMapping,
		VkFormat Format,  //因为我们无法知晓具体Format是否使用srgb，所以传入具体格式
		uint32_t FirstMip,
		uint32_t NumMips,
		uint32_t ArraySliceIndex,
		uint32_t NumArraySlices);


	~VulkanTextureView();

	VkImageView TextureImageView;
	VkImage TextureImage;
	VkDevice DeviceRef;
	//uint32_t ViewId;
};




#endif