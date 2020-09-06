#include <Public/VulkanRHI/VulkanPipeline.h>
#include <Public/VulkanRHI/VulkanRHI.h>
#include <Public/VulkanRHI/VulkanDevice.h>
#include <Public/VulkanRHI/VulkanSwapChain.h>
#include <Public/VulkanRHI/VulkanQueue.h>

#include <Public/Scene/RenderScene.h>
#include <Public/Basic/Camera/Camera.h>
#include <Public/Basic/File/File.h>
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

	//Index Buffer
	std::vector<uint32_t> IndexBuffer = { 0, 1, 2 };

	//Vertex Buffer
	std::vector<std::vector<Vector3>> VertexBuffers =
	{
		{
			Vector3(1.0f,  1.0f, 0.0f),
			Vector3(1.0f,  1.0f, 0.0f),
			Vector3(1.0f,  1.0f, 0.0f)
		},
		{
			Vector3(1.0f, 0.0f, 0.0f),
			Vector3(0.0f, 1.0f, 0.0f),
			Vector3(0.0f, 0.0f, 1.0f)
		}
	};


	//CreateCommandBuffer from the CommandPool
	{
		// Create one command buffer for each swap chain image and reuse for rendering
		BackCommandBuffers.resize(static_cast<uint32_t>(VulkanSwapChain::BackBufferSize::NUM_BUFFERS));
		VkCommandBufferAllocateInfo CommandBufferAllocateInfo{};
		CommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		CommandBufferAllocateInfo.commandPool = Device->GetCommandBufferPool(CommandPoolType::GfxPool);
		CommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		CommandBufferAllocateInfo.commandBufferCount = static_cast<uint32_t>(BackCommandBuffers.size());
		assert(vkAllocateCommandBuffers(Device->GetLogicDevice(), &CommandBufferAllocateInfo, BackCommandBuffers.data()) == VK_SUCCESS);
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
		BackBufferSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		//VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT 决定command buffer 什么时候发生等待
		//pWaitDstStageMask指定了队列提交会进行等待的管道阶段
		VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		BackBufferSubmitInfo.pWaitDstStageMask = &submitPipelineStages;
		BackBufferSubmitInfo.waitSemaphoreCount = 1;
		BackBufferSubmitInfo.pWaitSemaphores = &semaphores.presentComplete; //决定起始地址的wait semaphore数组
		BackBufferSubmitInfo.signalSemaphoreCount = 1;
		BackBufferSubmitInfo.pSignalSemaphores = &semaphores.renderComplete; //决定起始地址的signal semaphore数组
	}

	//Create Fence
	{
		// Wait fences to sync command buffer access
		VkFenceCreateInfo FenceCreateInfo{};
		FenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		FenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; //初始化为sig状态
		BackCommandWaitFences.resize(BackCommandBuffers.size());
		for (auto& Fence : BackCommandWaitFences) {
			//每个CommandBuffer创建一个Fence
			assert(vkCreateFence(Device->GetLogicDevice(), &FenceCreateInfo, nullptr, &Fence) == VK_SUCCESS);
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

		assert(vkCreateRenderPass(Device->GetLogicDevice(), &renderPassInfo, nullptr, &TriangleRenderPass) == VK_SUCCESS);
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
			frameBufferCreateInfo.renderPass = TriangleRenderPass;
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
		VulkanIndexBufferResource StagingIndexBuffer;
		std::vector<VulkanVertexBufferResource> StagingVertexBuffers(VertexBuffers.size());
		TriangleVertexBuffer.resize(VertexBuffers.size());

		StagingIndexBuffer.CreateBuffer<true>(
			Device,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			static_cast<uint32_t>(IndexBuffer.size() * sizeof(uint32_t)),
			IndexBuffer.data()
		);

		TriangleIndexBuffer.CreateBuffer<false>(
			Device,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			static_cast<uint32_t>(IndexBuffer.size() * sizeof(uint32_t)),
			nullptr
		);

		for (auto index = 0; index < VertexBuffers.size(); ++index) {
			StagingVertexBuffers[index].CreateBuffer<true>(
				Device,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				static_cast<uint32_t>(VertexBuffers[index].size() * sizeof(Vector3)),
				VertexBuffers[index].data()
			);

			TriangleVertexBuffer[index].CreateBuffer<false>(
				Device,
				VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				static_cast<uint32_t>(VertexBuffers[index].size() * sizeof(Vector3)),
				nullptr
			);
		}

		//Execute copy, can also use transfer queue
		VkCommandPool GfxPool = Device->GetCommandBufferPool(CommandPoolType::GfxPool);
		VkCommandBuffer CopyCmdBuffer = nullptr;
		Device->AllocateCommandBuffer(CommandPoolType::GfxPool, 1, &CopyCmdBuffer);
		VkCommandBufferBeginInfo cmdBufferBeginInfo{};
		cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		assert(vkBeginCommandBuffer(CopyCmdBuffer, &cmdBufferBeginInfo) == VK_SUCCESS);
		{
			//#TODO: use transfer queue
			VkBufferCopy copyRegion = {};
			for (auto index = 0; index < VertexBuffers.size(); ++index) {
				copyRegion.size = static_cast<uint32_t>(VertexBuffers[index].size() * sizeof(Vector3));
				vkCmdCopyBuffer(CopyCmdBuffer, StagingVertexBuffers[index].ResourceBuffer, TriangleVertexBuffer[index].ResourceBuffer, 1, &copyRegion);
			}
			copyRegion.size = static_cast<uint32_t>(IndexBuffer.size() * sizeof(uint32_t));
			vkCmdCopyBuffer(CopyCmdBuffer, StagingIndexBuffer.ResourceBuffer, TriangleIndexBuffer.ResourceBuffer, 1, &copyRegion);
		}
		assert(vkEndCommandBuffer(CopyCmdBuffer) == VK_SUCCESS);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &CopyCmdBuffer;

		//VkFenceCreateInfo fenceCreateInfo = {};
		//fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		//fenceCreateInfo.flags = 0;
		//VkFence fence;
		//VK_CHECK_RESULT(vkCreateFence(device, &fenceCreateInfo, nullptr, &fence));

		assert(vkQueueSubmit(Device->GetGfxQueue()->GetQueue(), 1, &submitInfo, VK_NULL_HANDLE) == VK_SUCCESS);
		assert(vkQueueWaitIdle(Device->GetGfxQueue()->GetQueue()) == VK_SUCCESS);

		vkFreeCommandBuffers(Device->GetLogicDevice(), GfxPool, 1, &CopyCmdBuffer);
		//Release staging Buffers
		StagingIndexBuffer.ReleaseBuffer(Device->GetLogicDevice());
		for (auto index = 0; index < VertexBuffers.size(); ++index) {
			StagingVertexBuffers[index].ReleaseBuffer(Device->GetLogicDevice());
		}
	}

	//Create Uniform Buffer
	{
		TriangleTransformUB.CreateBuffer<true>(
			Device,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			static_cast<uint32_t>(sizeof(PassViewUniformBuffer)),
			0,
			nullptr
		);
	}


	//Create PipeLine
	{
		// Construct the different states making up the pipeline

		// Input assembly state describes how primitives are assembled
		// This pipeline will assemble vertex data as a triangle lists (though we only use one triangle)
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
		inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		// Rasterization state
		VkPipelineRasterizationStateCreateInfo rasterizationState = {};
		rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationState.polygonMode = VK_POLYGON_MODE_FILL; //
		rasterizationState.cullMode = VK_CULL_MODE_NONE;  //Face And Back
		rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizationState.depthClampEnable = VK_FALSE;
		rasterizationState.rasterizerDiscardEnable = VK_FALSE;
		rasterizationState.depthBiasEnable = VK_FALSE;
		rasterizationState.lineWidth = 1.0f;

		// Color blend state describes how blend factors are calculated (if used)
		// We need one blend attachment state per color attachment (even if blending is not used)
		VkPipelineColorBlendAttachmentState blendAttachmentState[1] = {};
		blendAttachmentState[0].colorWriteMask = 0xf;
		blendAttachmentState[0].blendEnable = VK_FALSE;
		VkPipelineColorBlendStateCreateInfo colorBlendState = {};
		colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendState.attachmentCount = 1;
		colorBlendState.pAttachments = blendAttachmentState;

		// Viewport state sets the number of viewports and scissor used in this pipeline
		// Note: This is actually overridden by the dynamic states (see below)
		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		// Enable dynamic states
		// Most states are baked into the pipeline, but there are still a few dynamic states that can be changed within a command buffer
		// To be able to change these we need do specify which dynamic states will be changed using this pipeline. Their actual states are set later on in the command buffer.
		// For this example we will set the viewport and scissor using dynamic states
		std::vector<VkDynamicState> dynamicStateEnables;
		dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);
		dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);
		VkPipelineDynamicStateCreateInfo dynamicState = {};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.pDynamicStates = dynamicStateEnables.data();
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());

		// Depth and stencil state containing depth and stencil compare and test operations
		// We only use depth tests and want depth tests and writes to be enabled and compare with less or equal
		VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
		depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilState.depthTestEnable = VK_TRUE;
		depthStencilState.depthWriteEnable = VK_TRUE;
		depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		depthStencilState.depthBoundsTestEnable = VK_FALSE;//深度范围测试

		//Stencil oprator
		depthStencilState.stencilTestEnable = VK_FALSE;
		depthStencilState.back.failOp = VK_STENCIL_OP_KEEP;
		depthStencilState.back.passOp = VK_STENCIL_OP_KEEP;
		depthStencilState.back.compareOp = VK_COMPARE_OP_ALWAYS;
		depthStencilState.front = depthStencilState.back;

		// Multi sampling state
		// This example does not make use of multi sampling (for anti-aliasing), the state must still be set and passed to the pipeline
		VkPipelineMultisampleStateCreateInfo multisampleState = {};
		multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampleState.pSampleMask = nullptr;

		// Vertex input descriptions
		// Specifies the vertex input parameters for a pipeline
		// These match the following shader layout (see triangle.vert):
		//	layout (location = 0) in vec3 inPos;
		//	layout (location = 1) in vec3 inColor;
		std::array<VkVertexInputBindingDescription, 2> VertexInputBindDes;
		std::array<VkVertexInputAttributeDescription, 2> VertexInputAttributs;

		//Position
		VertexInputBindDes[0].binding = 0;
		VertexInputBindDes[0].stride = sizeof(Vector3);
		VertexInputBindDes[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		// Attribute location 0: Position
		VertexInputAttributs[0].binding = 0;
		VertexInputAttributs[0].location = 0;
		VertexInputAttributs[0].format = VK_FORMAT_R32G32B32_SFLOAT;// Position attribute is three 32 bit signed (SFLOAT) floats (R32 G32 B32)
		VertexInputAttributs[0].offset = 0;

		//Vertex Color
		VertexInputBindDes[0].binding = 1;
		VertexInputBindDes[0].stride = sizeof(Vector3);
		VertexInputBindDes[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		// Attribute location 1: Color
		VertexInputAttributs[1].binding = 1;
		VertexInputAttributs[1].location = 1;
		VertexInputAttributs[1].format = VK_FORMAT_R32G32B32_SFLOAT;// Color attribute is three 32 bit signed (SFLOAT) floats (R32 G32 B32)
		VertexInputAttributs[1].offset = 0;


		// Vertex input state used for pipeline creation
		VkPipelineVertexInputStateCreateInfo vertexInputState = {};
		vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputState.vertexBindingDescriptionCount = static_cast<uint32_t>(VertexInputBindDes.size());
		vertexInputState.pVertexBindingDescriptions = VertexInputBindDes.data();
		vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(VertexInputAttributs.size());
		vertexInputState.pVertexAttributeDescriptions = VertexInputAttributs.data();

		//Shader Layout, uniformbuffer texture etc...
		VkDescriptorSetLayoutBinding layoutBinding = {};
		layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		layoutBinding.descriptorCount = 1;
		layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		layoutBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo descriptorLayout = {};
		descriptorLayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorLayout.pNext = nullptr;
		descriptorLayout.bindingCount = 1;
		descriptorLayout.pBindings = &layoutBinding;
		assert(vkCreateDescriptorSetLayout(Device->GetLogicDevice(), &descriptorLayout, nullptr, &TriangleDscLayout) == VK_SUCCESS);

		// Create the pipeline layout that is used to generate the rendering pipelines that are based on this descriptor set layout
		// In a more complex scenario you would have different pipeline layouts for different descriptor set layouts that could be reused
		VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
		pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pPipelineLayoutCreateInfo.pNext = nullptr;
		pPipelineLayoutCreateInfo.setLayoutCount = 1;
		pPipelineLayoutCreateInfo.pSetLayouts = &TriangleDscLayout;
		assert(vkCreatePipelineLayout(Device->GetLogicDevice(), &pPipelineLayoutCreateInfo, nullptr, &TrianglePipelineLayout) == VK_SUCCESS);


		// Shaders
		std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages{};
		// Vertex shader
		shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module = RHI->LoadSpvShader(CoreDefine::AssetPath + "VulkanShaders/Triangle/triangleVert.spv");
		shaderStages[0].pName = "main";
		assert(shaderStages[0].module != VK_NULL_HANDLE);

		// Fragment shader
		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[1].module = RHI->LoadSpvShader(CoreDefine::AssetPath + "VulkanShaders/Triangle/triangleVert.spv");
		shaderStages[1].pName = "main";
		assert(shaderStages[1].module != VK_NULL_HANDLE);

		// Set pipeline shader stage info
		VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.layout = TrianglePipelineLayout;
		pipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
		pipelineCreateInfo.pStages = shaderStages.data();
		pipelineCreateInfo.pVertexInputState = &vertexInputState;
		pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
		pipelineCreateInfo.pRasterizationState = &rasterizationState;
		pipelineCreateInfo.pColorBlendState = &colorBlendState;
		pipelineCreateInfo.pMultisampleState = &multisampleState;
		pipelineCreateInfo.pViewportState = &viewportState;
		pipelineCreateInfo.pDepthStencilState = &depthStencilState;
		pipelineCreateInfo.renderPass = TriangleRenderPass;
		pipelineCreateInfo.pDynamicState = &dynamicState;

		// Create rendering pipeline using the specified states
		assert(vkCreateGraphicsPipelines(Device->GetLogicDevice(), pipelineCache, 1, &pipelineCreateInfo, nullptr, &TrianglePipeline) == VK_SUCCESS);

		// Shader modules are no longer needed once the graphics pipeline has been created
		vkDestroyShaderModule(Device->GetLogicDevice(), shaderStages[0].module, nullptr);
		vkDestroyShaderModule(Device->GetLogicDevice(), shaderStages[1].module, nullptr);
	}

	//创建DescriptorPool,这里只有一个UniformBuffer
	{
		VkDescriptorPoolSize descriptorPoolSize{};
		descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorPoolSize.descriptorCount = 1;

		VkDescriptorPoolCreateInfo descriptorPoolInfo{};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.poolSizeCount = 1; //一种VkDescriptorType
		descriptorPoolInfo.pPoolSizes = &descriptorPoolSize;
		descriptorPoolInfo.maxSets = 1; //看VkDescriptorSet数量
		assert(vkCreateDescriptorPool(Device->GetLogicDevice(), &descriptorPoolInfo, nullptr, &TriangleDescriptorPool) == VK_SUCCESS);
	}
}

VulkanPipeline::~VulkanPipeline() {

	VulkanDevice* Device = RHI->GetDevice();
	VkDevice LogicDevice = Device->GetLogicDevice();
	delete SwapChain;
	SwapChainDepthStencilResource.ReleaseRenderResource(Device);

	TriangleIndexBuffer.ReleaseBuffer(LogicDevice);
	for (auto& VertexBufferData : TriangleVertexBuffer) {
		VertexBufferData.ReleaseBuffer(LogicDevice);
	}

	vkDestroyRenderPass(LogicDevice, TriangleRenderPass, nullptr);
	for (uint32_t i = 0; i < SwapChainFrameBuffers.size(); i++) {
		vkDestroyFramebuffer(LogicDevice, SwapChainFrameBuffers[i], nullptr);
	}
	vkFreeCommandBuffers(LogicDevice, Device->GetCommandBufferPool(CommandPoolType::GfxPool), static_cast<uint32_t>(BackCommandBuffers.size()), BackCommandBuffers.data());

	vkDestroySemaphore(LogicDevice, semaphores.presentComplete, nullptr);
	vkDestroySemaphore(LogicDevice, semaphores.renderComplete, nullptr);
	for (auto& fence : BackCommandWaitFences) {
		vkDestroyFence(LogicDevice, fence, nullptr);
	}
}


void VulkanPipeline::BeginFrame(RenderScene* World){
	UpdateUniformBuffer(World);
}

void VulkanPipeline::Render(){

}

void VulkanPipeline::EndFrame(){

}

void VulkanPipeline::UpdateUniformBuffer(RenderScene* World){
	VulkanDevice* Device = RHI->GetDevice();
	VkDevice LogicDevice = Device->GetLogicDevice();

	PassViewUniformBuffer ViewUniformBuffer(World->GetCamera()->GetViewMatrix(), World->GetCamera()->GetProjectMatrix());
	TriangleTransformUB.UpdateBuffer(LogicDevice, &ViewUniformBuffer);
}

