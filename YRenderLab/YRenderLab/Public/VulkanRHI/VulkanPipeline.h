#ifndef _YRENDER_VKRHI_VULKANPIPELINE_H_
#define _YRENDER_VKRHI_VULKANPIPELINE_H_


#include <Public/VulkanRHI/VulkanRHI.h>
#include <Public/VulkanRHI/VulkanResources.h>

class IPipeline;
class VulkanSwapChain;


class VulkanPipeline : public IPipeline {
	struct VulkanFrameSemaphores {
		VkSemaphore presentComplete;// Swap chain image presentation
		VkSemaphore renderComplete;// Command buffer submission and execution
	};


public:
	virtual ~VulkanPipeline();
	VulkanPipeline(void* InWindowHandle, VulkanRHI* InRHI, uint32_t InSizeX, uint32_t InSizeY, EPixelFormat InPixelFormat, bool bIsSRGB);

	virtual void BeginFrame() override;
	virtual void Render() override;
	virtual void EndFrame() override;

private:
	void* WindowHandle;
	//[Resource ref]
	VulkanRHI* RHI;
	uint32_t SizeX;
	uint32_t SizeY;
	EPixelFormat PixelFormat;

	//[Resource management]
	VulkanSwapChain* SwapChain;
	std::vector<VkCommandBuffer> BackCommandBuffers;
	// Synchronization semaphores
	VulkanFrameSemaphores semaphores;
	// Contains command buffers and semaphores to be presented to the queue
	VkSubmitInfo BackBufferSubmitInfo;
	std::vector<VkFence> BackCommandWaitFences;

	//[Resource Build]
	VulkanTextureResource SwapChainDepthStencilResource;
	// Global render pass for frame buffer writes
	VkRenderPass BackBufferRenderPass;
	// Pipeline cache object
	VkPipelineCache pipelineCache;
	// List of available frame buffers (same as number of swap chain images)
	std::vector<VkFramebuffer> SwapChainFrameBuffers;

	//All buffers
	VulkanIndexBufferResource TriangleIndexBuffer;
	std::vector<VulkanVertexBufferResource> TriangleVertexBuffer;
	VulkanUniformBufferResource TriangleTransformUB;
};


#endif