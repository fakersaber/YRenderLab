#include <Public/VulkanRHI/VulkanPipeline.h>
#include <Public/VulkanRHI/VulkanRHI.h>
#include <Public/VulkanRHI/VulkanSwapChain.h>
#include <Public/VulkanRHI/VulkanResources.h>


VulkanPipeline::VulkanPipeline(void* InWindowHandle, VulkanRHI* InRHI, uint32_t InSizeX, uint32_t InSizeY, EPixelFormat InPixelFormat, bool bIsSRGB)
	: WindowHandle(InWindowHandle)
	, RHI(InRHI)
	, SizeX(InSizeX)
	, SizeY(InSizeY)
	, PixelFormat(InPixelFormat)
{

	SwapChain = new VulkanSwapChain(InWindowHandle, InRHI->GetInstance(), *InRHI->GetDevice(), InPixelFormat, bIsSRGB, InSizeX, InSizeY, BackBufferImages);

	for (auto i = 0; i < BackBufferImages.size(); ++i) {

		VkFormat Format = static_cast<VkFormat>(bIsSRGB ? VulkanRHI::SRGBMapping(InPixelFormat) :VulkanRHI::PlatformFormats[InPixelFormat].PlatformFormat);

		VkComponentMapping ComponentMapping = InRHI->GetComponentMapping(InPixelFormat);

		BackBufferTextureViews.emplace_back(
			new VulkanTextureView(
				*InRHI->GetDevice(),
				BackBufferImages[i],
				VK_IMAGE_VIEW_TYPE_2D,
				VK_IMAGE_ASPECT_COLOR_BIT,
				ComponentMapping,
				Format,
				0,1,0,1
			)
		);
	}

	//#TODO: ×¢²áShader,layout,pso
	//BuildFrameResources();
	//BuildShadersAndInputLayout();
	//BuildRootSignature();
	//BuildPSOs();
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


