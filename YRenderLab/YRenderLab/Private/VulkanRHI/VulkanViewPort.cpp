#include <Public/VulkanRHI/VulkanViewport.h>
#include <Public/VulkanRHI/VulkanRHI.h>
#include <Public/VulkanRHI/VulkanSwapChain.h>

VulkanViewPort::VulkanViewPort(void* InWindowHandle, VulkanRHI* InRHI)
	: WindowHandle(InWindowHandle)
	, RHI(InRHI)
{
	SwapChain = new VulkanSwapChain(WindowHandle,RHI->GetInstance());
}

VulkanViewPort::~VulkanViewPort()
{
	delete SwapChain;
}

void VulkanViewPort::CreateSwapchain()
{

}
