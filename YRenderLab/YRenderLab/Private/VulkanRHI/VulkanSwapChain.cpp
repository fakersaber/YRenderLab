#include <Public/VulkanRHI/VulkanSwapChain.h>
#include <Public/VulkanRHI/VulkanRHI.h>


VulkanSwapChain::VulkanSwapChain(void* WindowHandle, VkInstance InInstance)
	: SwapChain(VK_NULL_HANDLE)
	, Surface(VK_NULL_HANDLE)
	, Instance(InInstance)
{
	VulkanPlatform::CreateSurface(WindowHandle, Instance, &Surface);

	//#TODO: Create SwapChain
}

VulkanSwapChain::~VulkanSwapChain()
{

	vkDestroySurfaceKHR(Instance, Surface, nullptr);

	//#TODO: Destroy SwapChain
}
