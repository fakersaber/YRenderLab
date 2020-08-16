#include <Public/VulkanRHI/VulkanResources.h>
#include <Public/VulkanRHI/VulkanDevice.h>



VulkanTextureView::VulkanTextureView(
	VulkanDevice& Device, 
	VkImage InImage, 
	VkImageViewType ViewType, 
	VkImageAspectFlags AspectFlags, 
	VkComponentMapping ComponentMapping,
	VkFormat Format,
	uint32_t FirstMip, 
	uint32_t NumMips, 
	uint32_t ArraySliceIndex,
	uint32_t NumArraySlices)
	:
	TextureImage(InImage),
	DeviceRef(Device.GetInstanceDevice())
{
	VkImageViewCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = InImage;
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format = Format;
	createInfo.components = ComponentMapping;

	createInfo.subresourceRange.aspectMask = AspectFlags;
	createInfo.subresourceRange.baseMipLevel = FirstMip;
	createInfo.subresourceRange.levelCount = NumMips;
	createInfo.subresourceRange.baseArrayLayer = ArraySliceIndex;

	switch (ViewType)
	{
	case VK_IMAGE_VIEW_TYPE_3D:
		createInfo.subresourceRange.layerCount = 1;
		break;
	case VK_IMAGE_VIEW_TYPE_CUBE:
		assert(NumArraySlices == 1);
		createInfo.subresourceRange.layerCount = 6;
		break;
	case VK_IMAGE_VIEW_TYPE_CUBE_ARRAY:
		createInfo.subresourceRange.layerCount = 6 * NumArraySlices;
		break;
	case VK_IMAGE_VIEW_TYPE_1D_ARRAY:
	case VK_IMAGE_VIEW_TYPE_2D_ARRAY:
		createInfo.subresourceRange.layerCount = NumArraySlices;
		break;
	default:
		createInfo.subresourceRange.layerCount = 1;
		break;
	}

	auto Result = vkCreateImageView(DeviceRef, &createInfo, nullptr, &TextureImageView);
	assert(VK_SUCCESS == Result);
}

VulkanTextureView::~VulkanTextureView()
{
	vkDestroyImageView(DeviceRef, TextureImageView, nullptr);
}
