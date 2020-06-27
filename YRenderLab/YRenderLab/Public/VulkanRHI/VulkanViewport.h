#ifndef _YRENDER_VKRHI_VKVIPORT_H_
#define _YRENDER_VKRHI_VKVIPORT_H_

class VulkanSwapChain;

class VulkanRHI;

class VulkanViewPort {
public:
	VulkanViewPort() = default;

	VulkanViewPort(void* InWindowHandle, VulkanRHI* InRHI);

	~VulkanViewPort();

	void CreateSwapchain();

private:
	void* WindowHandle;

	VulkanSwapChain* SwapChain;

	VulkanRHI* RHI;

	
};


#endif