#include <Public/VulkanRHI/VulkanPipeline.h>
#include <Public/VulkanRHI/VulkanRHI.h>
#include <Public/VulkanRHI/VulkanDevice.h>
#include <Public/VulkanRHI/VulkanSwapChain.h>
#include <Public/VulkanRHI/VulkanQueue.h>
#include <Public/VulkanRHI/VulkanResources.h>

#include <array>

//Implement the VertexBufferResource
struct TriangleVertexBufferResource : VulkanIndexBufferResource {
	TriangleVertexBufferResource()
		: VulkanIndexBufferResource()
	{
	}

	void InitialVertexBufferResource() {

	}

	void ReleaseRenderResource(VulkanDevice* Device) {
		VulkanIndexBufferResource::ReleaseRenderResource(Device);
	}
	
	//不在CPU存储数据，写入后就释放
	//std::vector<Vector3> VertexPos;
	//std::vector<Vector3> VertexColor;
};



VulkanPipeline::VulkanPipeline(void* InWindowHandle, VulkanRHI* InRHI, uint32_t InSizeX, uint32_t InSizeY, EPixelFormat InPixelFormat, bool bIsSRGB)
	: WindowHandle(InWindowHandle)
	, RHI(InRHI)
	, SizeX(InSizeX)
	, SizeY(InSizeY)
	, PixelFormat(InPixelFormat)
{
	//---------------Create SwapChain-----------------------//
	VulkanDevice* Device = InRHI->GetDevice();
	SwapChain = new VulkanSwapChain(InWindowHandle, InRHI->GetInstance(), *Device, InPixelFormat, bIsSRGB, InSizeX, InSizeY);


	//CreateCommandPool
	{
		VkCommandPoolCreateInfo cmdPoolInfo = {};
		cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cmdPoolInfo.queueFamilyIndex = SwapChain->GetPresentQueue()->GetFamilyIndex();
		cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		assert(vkCreateCommandPool(Device->GetLogicDevice(), &cmdPoolInfo, nullptr, &CommandlBufferPool) == VK_SUCCESS);
	}

	//CreateCommandBuffer from the CommandPool
	{
		// Create one command buffer for each swap chain image and reuse for rendering
		CommandBuffers.resize(static_cast<uint32_t>(VulkanSwapChain::BackBufferSize::NUM_BUFFERS));
		VkCommandBufferAllocateInfo CommandBufferAllocateInfo{};
		CommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		CommandBufferAllocateInfo.commandPool = CommandlBufferPool;
		CommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		CommandBufferAllocateInfo.commandBufferCount = static_cast<uint32_t>(CommandBuffers.size());
		assert(vkAllocateCommandBuffers(Device->GetLogicDevice(), &CommandBufferAllocateInfo, CommandBuffers.data()) == VK_SUCCESS);
	}


	//Create synchronization objects
	{
		//用以同步不同的queue之间，或者同一个queue不同的submission之间的执行顺序。
		VkSemaphoreCreateInfo semaphoreCreateInfo = {};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreCreateInfo.pNext = nullptr;
		// Create a semaphore used to synchronize image presentation
		// Ensures that the image is displayed before we start submitting new commands to the queue
		assert(vkCreateSemaphore(Device->GetLogicDevice(), &semaphoreCreateInfo, nullptr, &semaphores.presentComplete) == VK_SUCCESS);
		// Create a semaphore used to synchronize command submission
		// Ensures that the image is not presented until all commands have been submitted and executed
		assert(vkCreateSemaphore(Device->GetLogicDevice(), &semaphoreCreateInfo, nullptr, &semaphores.renderComplete) == VK_SUCCESS);
		// Set up submit info structure
		// Semaphores will stay the same during application lifetime
		// Command buffer submission info is set by each example
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		//VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT 决定command buffer 什么时候发生等待
		//pWaitDstStageMask指定了队列提交会进行等待的管道阶段
		VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		submitInfo.pWaitDstStageMask = &submitPipelineStages;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &semaphores.presentComplete; //决定起始地址的wait semaphore数组
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &semaphores.renderComplete; //决定起始地址的signal semaphore数组
	}

	//Create Fence
	{
		// Wait fences to sync command buffer access
		VkFenceCreateInfo FenceCreateInfo{};
		FenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		FenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		WaitFences.resize(CommandBuffers.size());
		for (auto& fence : WaitFences) {
			//每个CommandBuffer创建一个Fence
			assert(vkCreateFence(Device->GetLogicDevice(), &FenceCreateInfo, nullptr, &fence) == VK_SUCCESS);
		}
	}

	//Create DepthStencil
	{
		VkImageCreateInfo imageCI{};
		imageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCI.imageType = VK_IMAGE_TYPE_2D;
		imageCI.format = VulkanRHI::GetPlatformFormat(PF_DepthStencil);
		imageCI.extent = { SizeX, SizeY, 1 };
		imageCI.mipLevels = 1;
		imageCI.arrayLayers = 1;
		imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCI.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		SwapChainDepthStencilResource.InitialTextureResource(
			Device, 
			imageCI, 
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
			VK_IMAGE_VIEW_TYPE_2D,
			VulkanRHI::GetComponentMapping(PF_DepthStencil),
			0,
			1,
			0,
			1,
			VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT
		);
	}


	//Create RenderPass
	{
		std::array<VkAttachmentDescription, 2> attachments = {};
		// Color attachment
		attachments[0].format = SwapChain->GetSwapChainColorFormat();
		attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		//随便给个值因为后面VkAttachmentReference
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;	   // Layout at render pass start. Initial doesn't matter, so we use undefined
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;  // Layout to which the attachment is transitioned when the render pass is finished,
																	   // As we want to present the color buffer to the swapchain, we transition to PRESENT_KHR
																	   // Layout与usage描述范围不同
		// Depth attachment
		attachments[1].format = VulkanRHI::GetPlatformFormat(PF_DepthStencil);
		attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorReference = {};
		colorReference.attachment = 0;
		colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthReference = {};
		depthReference.attachment = 1;
		depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorReference;			// Reference to the color attachment in slot 0
		subpassDescription.pDepthStencilAttachment = &depthReference;	// Reference to the depth attachment in slot 1
		subpassDescription.inputAttachmentCount = 0;
		subpassDescription.pInputAttachments = nullptr;
		subpassDescription.preserveAttachmentCount = 0;
		subpassDescription.pPreserveAttachments = nullptr;
		subpassDescription.pResolveAttachments = nullptr;


		// Setup subpass dependencies
		// These will add the implicit attachment layout transitions specified by the attachment descriptions
		std::array<VkSubpassDependency, 2> dependencies;
		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;                             // Producer of the dependency
		dependencies[0].dstSubpass = 0;                                               // 0为Subpass的Index？ 因为只有一个subpass所以这里表示当前这个subpass
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; // SubPass加载前PipelineStage阶段，一般要上一个pass执行结束的话用VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT														 
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; // SubPass加载后PipelineStage的阶段
		dependencies[0].srcAccessMask = 0;                                            // Transition前状态
		dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;         // Transition后状态
		dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		dependencies[1].srcSubpass = 0;                                               // 同上
		dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;                             // Consumer are all commands outside of the renderpass
		dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; // Subpass执行前PipelineStage
		dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;          // Subpass执行后PipelineStage
		dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;         // 保存Subpass输出结果
		dependencies[1].dstAccessMask = 0;											  // 外部也没有任何资源依赖当前subpass，直接就Present了
		dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;


		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpassDescription;
		renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassInfo.pDependencies = dependencies.data();

		assert(vkCreateRenderPass(Device->GetLogicDevice(), &renderPassInfo, nullptr, &BackBufferRenderPass) == VK_SUCCESS);
	}


	//Create FrameBuffer
	{
		SwapChainFrameBuffers.resize(static_cast<uint32_t>(VulkanSwapChain::BackBufferSize::NUM_BUFFERS));
		const auto& BackBufferImageViews = SwapChain->GetBackBufferTextureView();
		for (size_t i = 0; i < SwapChainFrameBuffers.size(); i++)
		{
			std::array<VkImageView, 2> AttachmentViews;
			AttachmentViews[0] = BackBufferImageViews[i];							      // Color attachment is the view of the swapchain image
			AttachmentViews[1] = SwapChainDepthStencilResource.TextureImageView;         // Depth/Stencil attachment is the same for all frame buffers

			VkFramebufferCreateInfo frameBufferCreateInfo = {};
			frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			// All frame buffers use the same renderpass setup
			frameBufferCreateInfo.renderPass = BackBufferRenderPass;
			frameBufferCreateInfo.attachmentCount = static_cast<uint32_t>(AttachmentViews.size());
			frameBufferCreateInfo.pAttachments = AttachmentViews.data();
			frameBufferCreateInfo.width = SizeX;
			frameBufferCreateInfo.height = SizeY;
			frameBufferCreateInfo.layers = 1;
			// Create the framebuffer
			assert(vkCreateFramebuffer(Device->GetLogicDevice(), &frameBufferCreateInfo, nullptr, &SwapChainFrameBuffers[i]) == VK_SUCCESS);
		}
	}

	//Create StaginBuffer for VB IB
	{
		//Vertex Buffer
		std::vector<Vertex> vertexBuffer =
		{
			{ {  1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
			{ { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
			{ {  0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } }
		};

		//Index Buffer
		std::vector<uint32_t> indexBuffer = { 0, 1, 2 };

		VulkanIndexBufferResource StagingIndexBuffer;
		StagingIndexBuffer.InitialIndexBufferResource<true>(
			Device, 
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
			static_cast<uint32_t>(indexBuffer.size()),indexBuffer);

		TriangleIndexBuffer.InitialIndexBufferResource<false>(Device, 
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			0, std::vector<uint32_t>())


		



		// Vertex buffer
		VkBufferCreateInfo vertexBufferInfo = {};
		vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		vertexBufferInfo.size = vertexBufferSize;
		// Buffer is used as the copy source
		vertexBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		// Create a host-visible buffer to copy the vertex data to (staging buffer)
		VK_CHECK_RESULT(vkCreateBuffer(device, &vertexBufferInfo, nullptr, &stagingBuffers.vertices.buffer));
		vkGetBufferMemoryRequirements(device, stagingBuffers.vertices.buffer, &memReqs);
		memAlloc.allocationSize = memReqs.size;
		// Request a host visible memory type that can be used to copy our data do
		// Also request it to be coherent, so that writes are visible to the GPU right after unmapping the buffer
		memAlloc.memoryTypeIndex = getMemoryTypeIndex(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		VK_CHECK_RESULT(vkAllocateMemory(device, &memAlloc, nullptr, &stagingBuffers.vertices.memory));
		// Map and copy
		VK_CHECK_RESULT(vkMapMemory(device, stagingBuffers.vertices.memory, 0, memAlloc.allocationSize, 0, &data));
		memcpy(data, vertexBuffer.data(), vertexBufferSize);
		vkUnmapMemory(device, stagingBuffers.vertices.memory);
		VK_CHECK_RESULT(vkBindBufferMemory(device, stagingBuffers.vertices.buffer, stagingBuffers.vertices.memory, 0));

		// Create a device local buffer to which the (host local) vertex data will be copied and which will be used for rendering
		vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		VK_CHECK_RESULT(vkCreateBuffer(device, &vertexBufferInfo, nullptr, &vertices.buffer));
		vkGetBufferMemoryRequirements(device, vertices.buffer, &memReqs);
		memAlloc.allocationSize = memReqs.size;
		memAlloc.memoryTypeIndex = getMemoryTypeIndex(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		VK_CHECK_RESULT(vkAllocateMemory(device, &memAlloc, nullptr, &vertices.memory));
		VK_CHECK_RESULT(vkBindBufferMemory(device, vertices.buffer, vertices.memory, 0));





		// Buffer copies have to be submitted to a queue, so we need a command buffer for them
		// Note: Some devices offer a dedicated transfer queue (with only the transfer bit set) that may be faster when doing lots of copies
		VkCommandBuffer copyCmd = getCommandBuffer(true);

		// Put buffer region copies into command buffer
		VkBufferCopy copyRegion = {};

		// Vertex buffer
		copyRegion.size = vertexBufferSize;
		vkCmdCopyBuffer(copyCmd, stagingBuffers.vertices.buffer, vertices.buffer, 1, &copyRegion);
		// Index buffer
		copyRegion.size = indexBufferSize;
		vkCmdCopyBuffer(copyCmd, stagingBuffers.indices.buffer, indices.buffer, 1, &copyRegion);

		// Flushing the command buffer will also submit it to the queue and uses a fence to ensure that all commands have been executed before returning
		flushCommandBuffer(copyCmd);

		// Destroy staging buffers
		// Note: Staging buffer must not be deleted before the copies have been submitted and executed
		vkDestroyBuffer(device, stagingBuffers.vertices.buffer, nullptr);
		vkFreeMemory(device, stagingBuffers.vertices.memory, nullptr);
		vkDestroyBuffer(device, stagingBuffers.indices.buffer, nullptr);
		vkFreeMemory(device, stagingBuffers.indices.memory, nullptr);

	}
}

VulkanPipeline::~VulkanPipeline() {

	auto LogicDevice = RHI->GetDevice()->GetLogicDevice();

	delete SwapChain;

	SwapChainDepthStencilResource.ReleaseRenderResource(RHI->GetDevice());
	TriangleIndexBuffer.ReleaseRenderResource(RHI->GetDevice());

	vkDestroyRenderPass(LogicDevice, BackBufferRenderPass, nullptr);
	for (uint32_t i = 0; i < SwapChainFrameBuffers.size(); i++) {
		vkDestroyFramebuffer(LogicDevice, SwapChainFrameBuffers[i], nullptr);
	}
	vkFreeCommandBuffers(LogicDevice, CommandlBufferPool, static_cast<uint32_t>(CommandBuffers.size()), CommandBuffers.data());
	vkDestroyCommandPool(LogicDevice, CommandlBufferPool, nullptr);
	vkDestroySemaphore(LogicDevice, semaphores.presentComplete, nullptr);
	vkDestroySemaphore(LogicDevice, semaphores.renderComplete, nullptr);
	for (auto& fence : WaitFences) {
		vkDestroyFence(LogicDevice, fence, nullptr);
	}
}


void VulkanPipeline::BeginFrame()
{
}

void VulkanPipeline::Render()
{

}

void VulkanPipeline::EndFrame()
{

}

