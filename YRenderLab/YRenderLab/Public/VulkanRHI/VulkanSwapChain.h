#ifndef _YRENDER_VKRHI_VKSWAPCHAIN_H_
#define _YRENDER_VKRHI_VKSWAPCHAIN_H_

#include <vulkan/vulkan.h>
#include <Public/RHI/RHI.h>

class VulkanDevice;

class VulkanSwapChain
{
public:
	VulkanSwapChain() = delete;

	VulkanSwapChain(void* WindowHandle, VkInstance InInstance, VulkanDevice& InDevice, EPixelFormat& InOutPixelFormat);

	~VulkanSwapChain();

private:
	VkSwapchainKHR SwapChain;

	VkSurfaceKHR Surface;

	VkInstance Instance;

	VulkanDevice& Device;
};

#endif
