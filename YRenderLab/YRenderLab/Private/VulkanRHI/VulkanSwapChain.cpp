#include <Public/VulkanRHI/VulkanSwapChain.h>
#include <Public/VulkanRHI/VulkanRHI.h>
#include <Public/VulkanRHI/VulkanDevice.h>
#include <Public/VulkanRHI/VulkanResources.h>


VulkanSwapChain::VulkanSwapChain(void* WindowHandle, VkInstance InInstance, VulkanDevice& InDevice, EPixelFormat& InOutPixelFormat, bool bIsSRGB, uint32_t Size_X, uint32_t Size_Y)
	: SwapChain(VK_NULL_HANDLE)
	, Surface(VK_NULL_HANDLE)
	, Instance(InInstance)
	, Device(InDevice)
	, PresentQueue(nullptr)
{
	//------------------------Create Current Platform window surface, note there are only windows now------------------------
	VulkanPlatform::CreateSurface(WindowHandle, Instance, &Surface);


	//------------------------fetch SurfaceFormats------------------------
	bool bFormatIsFound = false;
	uint32_t NumFormats;
	vkGetPhysicalDeviceSurfaceFormatsKHR(Device.GetPhysicalDevice(), Surface, &NumFormats, nullptr);
	assert(NumFormats > 0);
	std::vector<VkSurfaceFormatKHR> Formats;
	Formats.resize(NumFormats);
	vkGetPhysicalDeviceSurfaceFormatsKHR(Device.GetPhysicalDevice(), Surface, &NumFormats, Formats.data());
	for (uint32_t Index = 0; Index < NumFormats; ++Index) {
		bFormatIsFound = bIsSRGB ? 
			VulkanRHI::SRGBMapping(InOutPixelFormat) == Formats[Index].format : 
			VulkanRHI::GetPlatformFormat(InOutPixelFormat) == Formats[Index].format;

		if (bFormatIsFound) {
			SwapChainFormat = Formats[Index];
			break;
		}
		
	}
	assert(bFormatIsFound);

	//------------------------Fetch present mode------------------------
	//	// If v-sync is not requested, try to find a mailbox mode
	// It's the lowest latency non-tearing present mode available
	VkPresentModeKHR PresentMode = VK_PRESENT_MODE_MAILBOX_KHR; //默认使用三缓冲
	bool bPresentMode = false;
	uint32_t NumFoundPresentModes = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(Device.GetPhysicalDevice(), Surface, &NumFoundPresentModes, nullptr);
	assert(NumFoundPresentModes > 0);
	std::vector<VkPresentModeKHR> FoundPresentModes;
	FoundPresentModes.resize(NumFoundPresentModes);
	vkGetPhysicalDeviceSurfacePresentModesKHR(Device.GetPhysicalDevice(), Surface, &NumFoundPresentModes, FoundPresentModes.data());
	for (uint32_t i = 0; i < NumFoundPresentModes; ++i) {
		switch (PresentMode) {
		case VK_PRESENT_MODE_FIFO_KHR:
			bPresentMode = true;
			break;
		case VK_PRESENT_MODE_MAILBOX_KHR:
			bPresentMode = true;
			break;
		case VK_PRESENT_MODE_IMMEDIATE_KHR:  //即刻刷新
			bPresentMode = true;
			break;
		default:
			break;
		}
	}
	assert(bPresentMode);


	//------------------------Check the surface properties and formats------------------------
	//The swap extent is the resolution of the swap chain images
	VkSurfaceCapabilitiesKHR SurfProperties;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device.GetPhysicalDevice(),Surface, &SurfProperties);


	//------------------------Create SwapChain------------------------
	VkSwapchainCreateInfoKHR SwapChainInfo;
	std::memset(&SwapChainInfo, 0, sizeof(VkSwapchainCreateInfoKHR));
	SwapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	SwapChainInfo.surface = Surface;

	assert(SurfProperties.maxImageCount == 0);
	SwapChainInfo.minImageCount = SurfProperties.maxImageCount == 0 ?  //为0表示buffer没有限制
		static_cast<uint32_t>(BackBufferSize::NUM_BUFFERS) : 
		YGM::Math::Clamp(static_cast<uint32_t>(BackBufferSize::NUM_BUFFERS), SurfProperties.minImageCount, SurfProperties.maxImageCount); 

	assert(SwapChainInfo.minImageCount == static_cast<uint32_t>(BackBufferSize::NUM_BUFFERS));

	SwapChainInfo.imageFormat = SwapChainFormat.format;
	SwapChainInfo.imageColorSpace = SwapChainFormat.colorSpace;
	SwapChainInfo.imageExtent.width = SurfProperties.currentExtent.width == 0xFFFFFFFFul ? Size_X : SurfProperties.currentExtent.width;
	SwapChainInfo.imageExtent.height = SurfProperties.currentExtent.height == 0xFFFFFFFFul ? Size_Y : SurfProperties.currentExtent.height;

	//指定资源类型，这里仅指定为FrameBuffer(RTV)
	SwapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;  
	//SwapChainTransform信息，例如是否旋转等
	SwapChainInfo.preTransform = SurfProperties.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR ? VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR : SurfProperties.currentTransform;
	//The imageArrayLayers specifies the amount of layers each image consists of. This is always 1 unless you are developing a stereoscopic 3D application.
	SwapChainInfo.imageArrayLayers = 1;
	//we need to specify how to handle swap chain images that will be used across multiple queue families.
	SwapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	SwapChainInfo.presentMode = PresentMode;
	//Alpha混合方式，混合由前面Pass决定，SwapChain显然只显示RGB
	SwapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	SwapChainInfo.oldSwapchain = VK_NULL_HANDLE;

	//set Present
	PresentQueue = InDevice.SetupPresentQueue(Surface);

	//Create SwapChain
	VkResult Result = vkCreateSwapchainKHR(Device.GetLogicDevice(), &SwapChainInfo, nullptr, &SwapChain);
	assert(Result == VK_SUCCESS);

	//Retrieving the swap chain images
	uint32_t NumSwapChainImages;
	vkGetSwapchainImagesKHR(Device.GetLogicDevice(), SwapChain, &NumSwapChainImages, nullptr);
	BackBufferImages.resize(NumSwapChainImages);
	vkGetSwapchainImagesKHR(Device.GetLogicDevice(), SwapChain, &NumSwapChainImages, BackBufferImages.data());

	//Create BackBufferImageView
	VkComponentMapping ComponentMapping = Device.GetVulkanRHI()->GetComponentMapping(InOutPixelFormat);
	for (auto i = 0; i < BackBufferImages.size(); ++i) {
		//因为SwapChain的VkImage和VkDeviceMem是驱动创建的，所以这里不使用封装的VulkanTexture
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = BackBufferImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = SwapChainFormat.format;
		createInfo.components = ComponentMapping;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;
		assert(vkCreateImageView(Device.GetLogicDevice(), &createInfo, nullptr, &BackBufferTextureViews[i]) == VK_SUCCESS);
	}

}

VulkanSwapChain::~VulkanSwapChain(){
	//VkImage和VkDeviceMem由驱动释放
	for (auto& BackBufferImageView : BackBufferTextureViews) {
		vkDestroyImageView(Device.GetLogicDevice(), BackBufferImageView, nullptr);
	}
	vkDestroySurfaceKHR(Instance, Surface, nullptr);
	vkDestroySwapchainKHR(Device.GetLogicDevice(), SwapChain, nullptr);
}
