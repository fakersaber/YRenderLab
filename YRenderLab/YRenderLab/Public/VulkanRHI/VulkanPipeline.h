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

	struct PassViewUniformBuffer {
		PassViewUniformBuffer(Mat4f&& InViewMat, Mat4f&& InProjMat)
			: ViewMat(InViewMat)
			, ProjMat(InProjMat)
		{}

		Mat4f ViewMat;
		Mat4f ProjMat;
	};


public:
	virtual ~VulkanPipeline();
	VulkanPipeline(void* InWindowHandle, VulkanRHI* InRHI, uint32_t InSizeX, uint32_t InSizeY, EPixelFormat InPixelFormat, bool bIsSRGB);

	virtual void BeginFrame(RenderScene* World) override;
	virtual void Render() override;
	virtual void EndFrame() override;
	void UpdateUniformBuffer(RenderScene* World);

private:
	void* WindowHandle;
	//[Resource ref]
	VulkanRHI* RHI;
	uint32_t SizeX;
	uint32_t SizeY;
	EPixelFormat PixelFormat;
	uint32_t CurBackBufferIndex;
	VkDevice LogicDevice;

	//[Resource management]
	VulkanSwapChain* SwapChain;
	std::vector<VkCommandBuffer> BackCommandBuffers;
	// Synchronization semaphores
	VulkanFrameSemaphores BackBufferSemaphores;
	// Contains command buffers and semaphores to be presented to the queue
	VkSubmitInfo BackBufferSubmitInfo;
	std::vector<VkFence> BackCommandWaitFences;
	VulkanTextureResource SwapChainDepthStencilResource;
	// Global render pass for frame buffer writes
	VkRenderPass TriangleRenderPass;

	// List of available frame buffers (same as number of swap chain images)
	std::vector<VkFramebuffer> BackFrameBuffers;

	//All buffers
	VulkanIndexBufferResource TriangleIndexBuffer;
	std::vector<VulkanVertexBufferResource> TriangleVertexBuffer;
	VulkanUniformBufferResource TriangleTransformUB;

	VkDescriptorSetLayout TriangleDscLayout;
	VkPipelineLayout TrianglePipelineLayout;
	VkDescriptorSet TriangelDescriptorSet; //不需要手动再释放，因为是从Pool中创建
	VkDescriptorPool TriangleDescriptorPool;

	// Pipeline cache object
	VkPipelineCache TrianglePipelineCache;
	VkPipeline TrianglePipeline;
	
};


#endif