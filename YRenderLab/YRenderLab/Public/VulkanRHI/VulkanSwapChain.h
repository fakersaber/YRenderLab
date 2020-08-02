#ifndef _YRENDER_VKRHI_VKSWAPCHAIN_H_
#define _YRENDER_VKRHI_VKSWAPCHAIN_H_

#include <vulkan/vulkan.h>
#include <Public/RHI/RHI.h>

class VulkanDevice;

class VulkanSwapChain
{
public:
	enum class BackBufferSize : unsigned char{ NUM_BUFFERS = 3 };

	VulkanSwapChain() = delete;

	VulkanSwapChain(void* WindowHandle, VkInstance InInstance, VulkanDevice& InDevice, EPixelFormat& InOutPixelFormat, bool bIsSRGB, uint32_t Size_X, uint32_t Size_Y);

	~VulkanSwapChain();

private:
	VkSwapchainKHR SwapChain;

	VkSurfaceKHR Surface;

	VkInstance Instance;

	VulkanDevice& Device;
};

#endif
