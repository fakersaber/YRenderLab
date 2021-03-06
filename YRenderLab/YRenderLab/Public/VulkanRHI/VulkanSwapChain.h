#ifndef _YRENDER_VKRHI_VKSWAPCHAIN_H_
#define _YRENDER_VKRHI_VKSWAPCHAIN_H_

#include <vulkan/vulkan.h>
#include <Public/RHI/RHI.h>
#include <vector>

class VulkanDevice;
class VulkanQueue;

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
		uint32_t Size_Y
	);
	~VulkanSwapChain();

	inline VulkanQueue* GetPresentQueue() const { return  PresentQueue; }
	inline VkFormat GetSwapChainColorFormat() const { return SwapChainFormat.format; }
	inline VkSwapchainKHR GetSwapChain() const { return SwapChain; }
	inline const std::vector<VkImageView>& GetBackBufferTextureView() const {return BackBufferTextureViews;}


	VkResult QueuePresent(VkQueue Queue, uint32_t CurImgIndex, VkSemaphore WaitSemaphore);

private:
	//[Resource ref]
	VulkanDevice& Device;
	VkInstance Instance;
	VulkanQueue* PresentQueue;
	std::vector<VkImage> BackBufferImages;//��������

	//[Resource management]
	VkSwapchainKHR SwapChain;
	VkSurfaceKHR Surface;
	std::vector<VkImageView> BackBufferTextureViews;
	VkSurfaceFormatKHR SwapChainFormat;
};

#endif
