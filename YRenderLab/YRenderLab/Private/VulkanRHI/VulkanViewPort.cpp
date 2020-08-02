#include <Public/VulkanRHI/VulkanViewport.h>
#include <Public/VulkanRHI/VulkanRHI.h>
#include <Public/VulkanRHI/VulkanSwapChain.h>



VulkanViewPort::VulkanViewPort(void* InWindowHandle, VulkanRHI* InRHI, uint32_t InSizeX, uint32_t InSizeY, EPixelFormat InPixelFormat, bool bIsSRGB)
	: WindowHandle(InWindowHandle)
	, RHI(InRHI)
	, SizeX(InSizeX)
	, SizeY(InSizeY)
	, PixelFormat(InPixelFormat)
{
	SwapChain = new VulkanSwapChain(InWindowHandle, InRHI->GetInstance(), *InRHI->GetDevice(), InPixelFormat, bIsSRGB, InSizeX, InSizeY);
}



VulkanViewPort::~VulkanViewPort()
{
	delete SwapChain;
}



void VulkanViewPort::Init()
{
	
}


void VulkanViewPort::Shutdown()
{
	
}
