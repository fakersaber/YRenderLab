#ifndef _YRENDER_VKRHI_VKVIPORT_H_
#define _YRENDER_VKRHI_VKVIPORT_H_

#include <iostream>

class VulkanSwapChain;
class VulkanRHI;

class VulkanViewPort {
public:
	VulkanViewPort() = default;

	VulkanViewPort(void* InWindowHandle, VulkanRHI* InRHI, uint32_t InSizeX, uint32_t InSizeY);

	~VulkanViewPort();


private:
	void* WindowHandle;

	VulkanSwapChain* SwapChain;

	VulkanRHI* RHI;

	uint32_t SizeX;

	uint32_t SizeY;
};


#endif