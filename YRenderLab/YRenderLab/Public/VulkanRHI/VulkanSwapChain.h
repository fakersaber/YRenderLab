#ifndef _YRENDER_VKRHI_VKSWAPCHAIN_H_
#define _YRENDER_VKRHI_VKSWAPCHAIN_H_

#include <vulkan/vulkan.h>

class VulkanSwapChain
{
public:
	VulkanSwapChain() = delete;

	VulkanSwapChain(void* WindowHandle, VkInstance InInstance);

	~VulkanSwapChain();

private:
	VkSwapchainKHR SwapChain;

	VkSurfaceKHR Surface;

	VkInstance Instance;
};

#endif
