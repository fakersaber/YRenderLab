#include <Public/VulkanRHI/VulkanSwapChain.h>
#include <Public/VulkanRHI/VulkanRHI.h>
#include <Public/VulkanRHI/VulkanDevice.h>


VulkanSwapChain::VulkanSwapChain(void* WindowHandle, VkInstance InInstance, VulkanDevice& InDevice, EPixelFormat& InOutPixelFormat, bool bIsSRGB)
	: SwapChain(VK_NULL_HANDLE)
	, Surface(VK_NULL_HANDLE)
	, Instance(InInstance)
	, Device(InDevice)
{
	//Create Current Platform window surface
	VulkanPlatform::CreateSurface(WindowHandle, Instance, &Surface);

	VkSurfaceFormatKHR CurrFormat;
	bool bFormatIsFound = false;

	//fetch SurfaceFormats
	uint32_t NumFormats;
	vkGetPhysicalDeviceSurfaceFormatsKHR(Device.GetPhysicalHandle(), Surface, &NumFormats, nullptr);
	assert(NumFormats > 0);
	std::vector<VkSurfaceFormatKHR> Formats;
	Formats.resize(NumFormats);
	vkGetPhysicalDeviceSurfaceFormatsKHR(Device.GetPhysicalHandle(), Surface, &NumFormats, Formats.data());
	for (uint32_t Index = 0; Index < NumFormats; ++Index) {
		bFormatIsFound = bIsSRGB ? 
			VulkanRHI::SRGBMapping(InOutPixelFormat) == Formats[Index].format : 
			VulkanRHI::PlatformFormats[InOutPixelFormat].PlatformFormat == Formats[Index].format;

		if (bFormatIsFound)
			break;
	}
	assert(bFormatIsFound);
	//InDevice.SetupPresentQueue(Surface);

	//Fetch present mode, Android not support ? 
	uint32_t NumFoundPresentModes = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(Device.GetPhysicalHandle(), Surface, &NumFoundPresentModes, nullptr);
	assert(NumFoundPresentModes > 0);
	std::vector<VkPresentModeKHR> FoundPresentModes;
	FoundPresentModes.resize(NumFoundPresentModes);
	vkGetPhysicalDeviceSurfacePresentModesKHR(Device.GetPhysicalHandle(), Surface, &NumFoundPresentModes, FoundPresentModes.data());

	// Check the surface properties and formats
	VkSurfaceCapabilitiesKHR SurfProperties;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device.GetPhysicalHandle(),Surface, &SurfProperties);

}

VulkanSwapChain::~VulkanSwapChain()
{
	vkDestroySurfaceKHR(Instance, Surface, nullptr);

	//#TODO: Destroy SwapChain
}
