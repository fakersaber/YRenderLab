#ifndef _YRENDER_VKRHI_VKSWAPCHAIN_H_
#define _YRENDER_VKRHI_VKSWAPCHAIN_H_

#include <vulkan/vulkan.h>
#include <Public/RHI/RHI.h>
#include <vector>

class VulkanDevice;

class VulkanSwapChain
{
public:
	enum class BackBufferSize : unsigned char{ NUM_BUFFERS = 3 };

	VulkanSwapChain() = delete;

	VulkanSwapChain(
		void* WindowHandle, 
		VkInstance InInstance, 
		VulkanDevice& InDevice, 
		EPixelFormat& InOutPixelFormat, 
		bool bIsSRGB, 
		uint32_t Size_X, 
		uint32_t Size_Y,
		std::vector<VkImage>& BackBufferImages
	);

	~VulkanSwapChain();

private:

	VulkanDevice& Device;

	//------------------------Native Vulkan Type------------------------
	VkSwapchainKHR SwapChain;

	VkSurfaceKHR Surface;

	VkInstance Instance;
};

#endif
