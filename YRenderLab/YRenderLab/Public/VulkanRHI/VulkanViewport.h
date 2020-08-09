#ifndef _YRENDER_VKRHI_VKVIPORT_H_
#define _YRENDER_VKRHI_VKVIPORT_H_

#include <iostream>
#include <Public/RHI/RHI.h>


class VulkanSwapChain;
class VulkanRHI;



/*=============================================================================
 *	The following RHI functions must be called from the main thread.
 *=============================================================================*/
class VulkanViewPort : public  RHIViewport {
public:
	VulkanViewPort() = delete;
	virtual void Init() final override;
	virtual void Shutdown() final override;
	virtual void Draw() final override;

	VulkanViewPort(void* InWindowHandle, VulkanRHI* InRHI, uint32_t InSizeX, uint32_t InSizeY, EPixelFormat InPixelFormat, bool bIsSRGB);

	~VulkanViewPort();


private:
	void* WindowHandle;

	VulkanSwapChain* SwapChain;

	EPixelFormat PixelFormat;

	VulkanRHI* RHI;

	uint32_t SizeX;

	uint32_t SizeY;
};


#endif