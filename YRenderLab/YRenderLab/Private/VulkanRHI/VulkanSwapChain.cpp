#include <Public/VulkanRHI/VulkanSwapChain.h>
#include <Public/VulkanRHI/VulkanRHI.h>
#include <Public/VulkanRHI/VulkanDevice.h>


VulkanSwapChain::VulkanSwapChain(void* WindowHandle, VkInstance InInstance, VulkanDevice& InDevice, EPixelFormat& InOutPixelFormat, bool bIsSRGB, uint32_t Size_X, uint32_t Size_Y)
	: SwapChain(VK_NULL_HANDLE)
	, Surface(VK_NULL_HANDLE)
	, Instance(InInstance)
	, Device(InDevice)
{
	//------------------------Create Current Platform window surface, note there are only windows now------------------------
	VulkanPlatform::CreateSurface(WindowHandle, Instance, &Surface);


	//------------------------fetch SurfaceFormats------------------------
	VkSurfaceFormatKHR CurrFormat;
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
			VulkanRHI::PlatformFormats[InOutPixelFormat].PlatformFormat == Formats[Index].format;

		if (bFormatIsFound) {
			CurrFormat = Formats[Index];
			break;
		}
		
	}
	assert(bFormatIsFound);

	//------------------------Fetch present mode------------------------
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

	SwapChainInfo.minImageCount = SurfProperties.maxImageCount == 0 ?  //为0表示buffer没有限制
		static_cast<uint32_t>(BackBufferSize::NUM_BUFFERS) : 
		YGM::Math::Clamp(static_cast<uint32_t>(BackBufferSize::NUM_BUFFERS), SurfProperties.minImageCount, SurfProperties.maxImageCount); 

	SwapChainInfo.imageFormat = CurrFormat.format;
	SwapChainInfo.imageColorSpace = CurrFormat.colorSpace;
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
	InDevice.SetupPresentQueue(Surface);

	//Create SwapChain
	VkResult Result = vkCreateSwapchainKHR(Device.GetLogicDevice(), &SwapChainInfo, nullptr, &SwapChain);
	assert(Result == VK_SUCCESS);

	//Retrieving the swap chain images
	uint32_t NumSwapChainImages;
	vkGetSwapchainImagesKHR(Device.GetLogicDevice(), SwapChain, &NumSwapChainImages, nullptr);
	BackBufferImages.resize(NumSwapChainImages);
	vkGetSwapchainImagesKHR(Device.GetLogicDevice(), SwapChain, &NumSwapChainImages, BackBufferImages.data());

	//Create BackBufferImageView
	for (auto i = 0; i < BackBufferImages.size(); ++i) {
		VkFormat Format = static_cast<VkFormat>(bIsSRGB ? VulkanRHI::SRGBMapping(InPixelFormat) : VulkanRHI::PlatformFormats[InPixelFormat].PlatformFormat);
		VkComponentMapping ComponentMapping = InRHI->GetComponentMapping(InPixelFormat);
		BackBufferTextureViews.emplace_back(
			new VulkanTextureView(
				*InRHI->GetDevice(),
				BackBufferImages[i],
				VK_IMAGE_VIEW_TYPE_2D,
				VK_IMAGE_ASPECT_COLOR_BIT,
				ComponentMapping,
				Format,
				0, 1, 0, 1
			)
		);
	}

}

VulkanSwapChain::~VulkanSwapChain()
{
	vkDestroySurfaceKHR(Instance, Surface, nullptr);

	vkDestroySwapchainKHR(Device.GetLogicDevice(), SwapChain, nullptr);
}
