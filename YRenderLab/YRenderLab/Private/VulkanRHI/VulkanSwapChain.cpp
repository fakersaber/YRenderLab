#include <Public/VulkanRHI/VulkanSwapChain.h>
#include <Public/VulkanRHI/VulkanRHI.h>
#include <Public/VulkanRHI/VulkanDevice.h>


VulkanSwapChain::VulkanSwapChain(void* WindowHandle, VkInstance InInstance, VulkanDevice& InDevice, EPixelFormat& InOutPixelFormat)
	: SwapChain(VK_NULL_HANDLE)
	, Surface(VK_NULL_HANDLE)
	, Instance(InInstance)
	, Device(InDevice)
{
	//Create Current Platform window surface
	VulkanPlatform::CreateSurface(WindowHandle, Instance, &Surface);

	//fetch SurfaceFormats
	uint32_t NumFormats;
	vkGetPhysicalDeviceSurfaceFormatsKHR(Device.GetPhysicalHandle(), Surface, &NumFormats, nullptr);
	assert(NumFormats > 0);
	std::vector<VkSurfaceFormatKHR> Formats;
	Formats.resize(NumFormats);
	vkGetPhysicalDeviceSurfaceFormatsKHR(Device.GetPhysicalHandle(), Surface, &NumFormats, Formats.data());
	



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
