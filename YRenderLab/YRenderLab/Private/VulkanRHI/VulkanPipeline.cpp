#include <Public/VulkanRHI/VulkanPipeline.h>
#include <Public/VulkanRHI/VulkanRHI.h>
#include <Public/VulkanRHI/VulkanDevice.h>
#include <Public/VulkanRHI/VulkanSwapChain.h>
#include <Public/VulkanRHI/VulkanResources.h>



VulkanPipeline::VulkanPipeline(void* InWindowHandle, VulkanRHI* InRHI, uint32_t InSizeX, uint32_t InSizeY, EPixelFormat InPixelFormat, bool bIsSRGB)
	: WindowHandle(InWindowHandle)
	, RHI(InRHI)
	, SizeX(InSizeX)
	, SizeY(InSizeY)
	, PixelFormat(InPixelFormat)
{
	//Create SwapChain
	VulkanDevice* Device = InRHI->GetDevice();
	SwapChain = new VulkanSwapChain(InWindowHandle, InRHI->GetInstance(), *Device, InPixelFormat, bIsSRGB, InSizeX, InSizeY);



	// Create synchronization objects
	//用以同步不同的queue之间，或者同一个queue不同的submission之间的执行顺序。
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
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
	//即在这里就是等待所有presentComplete Signal 执行完毕,并且当当前Command buffer执行完时设置renderComplete Signal
	submitInfo.pWaitDstStageMask = &submitPipelineStages; 
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &semaphores.presentComplete; //决定起始地址的wait semaphore数组
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &semaphores.renderComplete; //决定起始地址的signal semaphore数组


	//


}

VulkanPipeline::~VulkanPipeline() {
	for (auto& BackBufferImageView : BackBufferTextureViews) {
		delete BackBufferImageView;
	}
	delete SwapChain;
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


