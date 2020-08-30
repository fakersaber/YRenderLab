#include <Public/VulkanRHI/VulkanPipeline.h>
#include <Public/VulkanRHI/VulkanRHI.h>
#include <Public/VulkanRHI/VulkanDevice.h>
#include <Public/VulkanRHI/VulkanSwapChain.h>
#include <Public/VulkanRHI/VulkanQueue.h>
#include <Public/VulkanRHI/VulkanResources.h>

#include <array>



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

	//CreateCommandBuffer
	{
		// Create one command buffer for each swap chain image and reuse for rendering
		CommandBuffers.resize(static_cast<uint32_t>(VulkanSwapChain::BackBufferSize::NUM_BUFFERS));
		VkCommandBufferAllocateInfo CommandBufferAllocateInfo{};
		CommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		CommandBufferAllocateInfo.commandPool = CommandlBufferPool;
		CommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		CommandBufferAllocateInfo.commandBufferCount = CommandBuffers.size();
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


	BuildRenderResource();
}

VulkanPipeline::~VulkanPipeline() {

	ReleaseBuildRenderResource();

	auto LogicDevice = RHI->GetDevice()->GetLogicDevice();

	delete SwapChain;

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

void VulkanPipeline::BuildRenderResource(){
	auto Device = RHI->GetDevice();

	//Create DepthStencil
	{
		VkImageCreateInfo imageCI{};
		imageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCI.imageType = VK_IMAGE_TYPE_2D;
		imageCI.format = static_cast<VkFormat>(VulkanRHI::PlatformFormats[PF_DepthStencil].PlatformFormat);
		imageCI.extent = { SizeX, SizeY, 1 };
		imageCI.mipLevels = 1;
		imageCI.arrayLayers = 1;
		imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCI.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		DepthStencilResource = new VulkanTextureResource(*Device, imageCI, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		VkImageViewCreateInfo imageViewCI{};
		imageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCI.image = DepthStencilResource->TextureImage;
		imageViewCI.format = imageCI.format;
		imageViewCI.subresourceRange.baseMipLevel = 0;
		imageViewCI.subresourceRange.levelCount = 1;
		imageViewCI.subresourceRange.baseArrayLayer = 0;
		imageViewCI.subresourceRange.layerCount = 1;
		imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
		DepthStencilResource->BuildTextureView(imageViewCI);
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
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // Layout at render pass start. Initial doesn't matter, so we use undefined
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;  // Layout to which the attachment is transitioned when the render pass is finished,
																	   // As we want to present the color buffer to the swapchain, we transition to PRESENT_KHR
		// Depth attachment
		attachments[1].format = static_cast<VkFormat>(VulkanRHI::PlatformFormats[PF_DepthStencil].PlatformFormat);
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
		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL; //Vulkan天生有Dependencies,后面做RenderGraph很方便
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		dependencies[1].srcSubpass = 0;
		dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;


		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpassDescription;
		renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassInfo.pDependencies = dependencies.data();

		assert(vkCreateRenderPass(Device->GetLogicDevice(), &renderPassInfo, nullptr, &renderPass) == VK_SUCCESS);
	}


	//Create FrameBuffer
	{
		VkImageView attachments[2];

		// Depth/Stencil attachment is the same for all frame buffers
		attachments[1] = depthStencil.view;

		VkFramebufferCreateInfo frameBufferCreateInfo = {};
		frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBufferCreateInfo.pNext = NULL;
		frameBufferCreateInfo.renderPass = renderPass;
		frameBufferCreateInfo.attachmentCount = 2;
		frameBufferCreateInfo.pAttachments = attachments;
		frameBufferCreateInfo.width = width;
		frameBufferCreateInfo.height = height;
		frameBufferCreateInfo.layers = 1;

		// Create frame buffers for every swap chain image
		frameBuffers.resize(swapChain.imageCount);
		for (uint32_t i = 0; i < frameBuffers.size(); i++)
		{
			attachments[0] = swapChain.buffers[i].view;
			VK_CHECK_RESULT(vkCreateFramebuffer(device, &frameBufferCreateInfo, nullptr, &frameBuffers[i]));
		}
	}
}

void VulkanPipeline::ReleaseBuildRenderResource(){
	delete DepthStencilResource;
}


