#include <Public/VulkanRHI/VulkanViewport.h>
#include <Public/VulkanRHI/VulkanRHI.h>
#include <Public/VulkanRHI/VulkanSwapChain.h>

VulkanViewPort::VulkanViewPort(void* InWindowHandle, VulkanRHI* InRHI, uint32_t InSizeX, uint32_t InSizeY)
	: WindowHandle(InWindowHandle)
	, RHI(InRHI)
	, SizeX(InSizeX)
	, SizeY(InSizeY)
{
	SwapChain = new VulkanSwapChain(WindowHandle,RHI->GetInstance());
}

VulkanViewPort::~VulkanViewPort()
{
	delete SwapChain;
}

