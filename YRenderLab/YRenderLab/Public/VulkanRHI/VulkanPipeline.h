#ifndef _YRENDER_VKRHI_VULKANPIPELINE_H_
#define _YRENDER_VKRHI_VULKANPIPELINE_H_


#include <Public/VulkanRHI/VulkanRHI.h>

class IPipeline;
class VulkanSwapChain;
class VulkanRHI;
class VulkanTextureView;


/*=============================================================================
 *	The following RHI functions must be called from the main thread.
 *=============================================================================*/
class VulkanPipeline : public IPipeline {
public:
	~VulkanPipeline();
	VulkanPipeline(void* InWindowHandle, VulkanRHI* InRHI, uint32_t InSizeX, uint32_t InSizeY, EPixelFormat InPixelFormat, bool bIsSRGB);

	virtual void BeginFrame() override;
	virtual void Render() override;
	virtual void EndFrame() override;

	
private:
	void* WindowHandle;
	VulkanSwapChain* SwapChain;
	VulkanRHI* RHI;
	EPixelFormat PixelFormat;
	uint32_t SizeX;
	uint32_t SizeY;
	std::vector<VkImage> BackBufferImages;
	std::vector<VulkanTextureView*> BackBufferTextureViews;
};


#endif