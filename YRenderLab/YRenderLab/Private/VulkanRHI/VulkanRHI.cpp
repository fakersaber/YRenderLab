#include <Public/VulkanRHI/VulkanRHI.h>
#include <Public/VulkanRHI/VulkanDevice.h>
#include <Public/VulkanRHI/VulkanPipeline.h>

#include <fstream>

PixelFormatInfo RHI::PlatformFormats[static_cast<unsigned int>(EPixelFormat::PF_MAX)];
VkComponentMapping VulkanRHI::PixelFormatComponentMapping[static_cast<unsigned int>(EPixelFormat::PF_MAX)];

VulkanRHI::VulkanRHI()
	: Instance(VK_NULL_HANDLE)
	, Device(nullptr)
{

}

VulkanRHI::~VulkanRHI() {
	Device.reset();

	RemoveDebugLayerCallback();
	vkDestroyInstance(Instance, nullptr);
}

void VulkanRHI::Init() {
	SetupFormat();
	CreateInstance();
	SelectAndInitDevice();
}

void VulkanRHI::Shutdown() {

}

void VulkanRHI::SetupFormat(){

	VulkanRHI::PlatformFormats[PF_B8G8R8A8].PlatformFormat = VK_FORMAT_B8G8R8A8_UNORM;
	SetComponentMapping(PF_B8G8R8A8, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);

	VulkanRHI::PlatformFormats[PF_R8G8B8A8].PlatformFormat = VK_FORMAT_R8G8B8A8_UNORM;
	SetComponentMapping(PF_R8G8B8A8, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);

	VulkanRHI::PlatformFormats[PF_FloatRGB].PlatformFormat = VK_FORMAT_B10G11R11_UFLOAT_PACK32;
	SetComponentMapping(PF_FloatRGB, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_ONE);

	VulkanRHI::PlatformFormats[PF_FloatRGBA].PlatformFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
	SetComponentMapping(PF_FloatRGBA, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);

	VulkanRHI::PlatformFormats[PF_DepthStencil].PlatformFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;
	SetComponentMapping(PF_DepthStencil, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY);

	VulkanRHI::PlatformFormats[PF_ShadowDepth].PlatformFormat = VK_FORMAT_D16_UNORM;
	SetComponentMapping(PF_ShadowDepth, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY);

	//VulkanRHI::PlatformFormats[PF_DepthStencil].PlatformFormat = VK_FORMAT_D24_UNORM_S8_UINT;
	//SetComponentMapping(PF_DepthStencil, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY);

	//VulkanRHI::PlatformFormats[PF_DepthStencil].PlatformFormat = VK_FORMAT_D16_UNORM_S8_UINT;
	//SetComponentMapping(PF_DepthStencil, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY);

	//VulkanRHI::PlatformFormats[PF_DepthStencil].PlatformFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;
	//SetComponentMapping(PF_DepthStencil, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY);
	
}


unsigned int VulkanRHI::SRGBMapping(EPixelFormat UEFormat) {
	VkFormat Format = static_cast<VkFormat>(VulkanRHI::PlatformFormats[UEFormat].PlatformFormat);
	switch (Format) {
	case VK_FORMAT_B8G8R8A8_UNORM:				Format = VK_FORMAT_B8G8R8A8_SRGB; break;
	case VK_FORMAT_A8B8G8R8_UNORM_PACK32:		Format = VK_FORMAT_A8B8G8R8_SRGB_PACK32; break;
	case VK_FORMAT_R8_UNORM:					Format = /*((GMaxRHIFeatureLevel <= ERHIFeatureLevel::ES3_1) ? VK_FORMAT_R8_UNORM : */VK_FORMAT_R8_SRGB; break;
	case VK_FORMAT_R8G8_UNORM:					Format = VK_FORMAT_R8G8_SRGB; break;
	case VK_FORMAT_R8G8B8_UNORM:				Format = VK_FORMAT_R8G8B8_SRGB; break;
	case VK_FORMAT_R8G8B8A8_UNORM:				Format = VK_FORMAT_R8G8B8A8_SRGB; break;
	case VK_FORMAT_BC1_RGB_UNORM_BLOCK:			Format = VK_FORMAT_BC1_RGB_SRGB_BLOCK; break;
	case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:		Format = VK_FORMAT_BC1_RGBA_SRGB_BLOCK; break;
	case VK_FORMAT_BC2_UNORM_BLOCK:				Format = VK_FORMAT_BC2_SRGB_BLOCK; break;
	case VK_FORMAT_BC3_UNORM_BLOCK:				Format = VK_FORMAT_BC3_SRGB_BLOCK; break;
	case VK_FORMAT_BC7_UNORM_BLOCK:				Format = VK_FORMAT_BC7_SRGB_BLOCK; break;
	case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:		Format = VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK; break;
	case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:	Format = VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK; break;
	case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:	Format = VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK; break;
	case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:		Format = VK_FORMAT_ASTC_4x4_SRGB_BLOCK; break;
	case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:		Format = VK_FORMAT_ASTC_5x4_SRGB_BLOCK; break;
	case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:		Format = VK_FORMAT_ASTC_5x5_SRGB_BLOCK; break;
	case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:		Format = VK_FORMAT_ASTC_6x5_SRGB_BLOCK; break;
	case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:		Format = VK_FORMAT_ASTC_6x6_SRGB_BLOCK; break;
	case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:		Format = VK_FORMAT_ASTC_8x5_SRGB_BLOCK; break;
	case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:		Format = VK_FORMAT_ASTC_8x6_SRGB_BLOCK; break;
	case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:		Format = VK_FORMAT_ASTC_8x8_SRGB_BLOCK; break;
	case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:		Format = VK_FORMAT_ASTC_10x5_SRGB_BLOCK; break;
	case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:		Format = VK_FORMAT_ASTC_10x6_SRGB_BLOCK; break;
	case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:		Format = VK_FORMAT_ASTC_10x8_SRGB_BLOCK; break;
	case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:		Format = VK_FORMAT_ASTC_10x10_SRGB_BLOCK; break;
	case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:		Format = VK_FORMAT_ASTC_12x10_SRGB_BLOCK; break;
	case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:		Format = VK_FORMAT_ASTC_12x12_SRGB_BLOCK; break;
	}
	return Format;
}

IPipeline* VulkanRHI::RHICreateRenderPipeline(void * WindowHandle, uint32_t SizeX, uint32_t SizeY, EPixelFormat PreferredPixelFormat)
{
	//SwapChain只支持FORMAT_B8G8R8A8，原因未知
	return new VulkanPipeline(WindowHandle, this, SizeX, SizeY, PreferredPixelFormat, true);
}

void VulkanRHI::SetComponentMapping(EPixelFormat UEFormat, VkComponentSwizzle r, VkComponentSwizzle g, VkComponentSwizzle b, VkComponentSwizzle a){
	VkComponentMapping& ComponentMapping = PixelFormatComponentMapping[UEFormat];
	ComponentMapping.r = r;
	ComponentMapping.g = g;
	ComponentMapping.b = b;
	ComponentMapping.a = a;
}

VkComponentMapping VulkanRHI::GetComponentMapping(const EPixelFormat UEFormat)
{
	return PixelFormatComponentMapping[UEFormat];
}

VkFormat VulkanRHI::GetPlatformFormat(const EPixelFormat UEFormat)
{
	return static_cast<VkFormat>(PlatformFormats[UEFormat].PlatformFormat);
}



void VulkanRHI::CreateInstance() {
	VkApplicationInfo AppInfo = {};
	AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	AppInfo.pApplicationName = "YRenderLab";
	AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	AppInfo.pEngineName = "YRenderLab";
	AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	AppInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo InstInfo = {};
	InstInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InstInfo.pApplicationInfo = &AppInfo;

	VulkanRHI::GetInstanceLayersAndExtensions(InstanceExtensions, InstanceLayers);

	InstInfo.enabledExtensionCount = static_cast<uint32_t>(InstanceExtensions.size());
	InstInfo.ppEnabledExtensionNames = InstInfo.enabledExtensionCount > 0 ? InstanceExtensions.data() : nullptr;
	InstInfo.enabledLayerCount = static_cast<uint32_t>(InstanceLayers.size());
	InstInfo.ppEnabledLayerNames = InstInfo.enabledLayerCount > 0 ? InstanceLayers.data() : nullptr;

	//Allocator always nullptr
	VkResult Result = vkCreateInstance(&InstInfo, nullptr, &Instance);
	if (Result != VK_SUCCESS) {
		std::cerr << "Instance Create Error" << std::endl;
	}

#if VULKAN_HAS_DEBUGGING_ENABLED
	SetupDebugLayerCallback();
#endif

}

void VulkanRHI::SelectAndInitDevice(){
	uint32_t GpuCount = 0;
	VkResult Result = vkEnumeratePhysicalDevices(Instance, &GpuCount, nullptr);
	if (Result != VK_SUCCESS){
		std::cerr << "Cannot find a compatible Vulkan device or driver." << std::endl;
	}

	if (GpuCount == 0) {
		std::cerr << "No GPU(s)/Driver(s) that support Vulkan were found" << std::endl;
	}

	std::vector<VkPhysicalDevice> PhysicalDevices(GpuCount);
	vkEnumeratePhysicalDevices(Instance, &GpuCount, PhysicalDevices.data());
	if (Result != VK_SUCCESS) {
		std::cerr << "Couldn't enumerate physical devices!" << std::endl;
	}

	for (auto& CurGpu : PhysicalDevices){
		auto NewDevice = std::make_shared<VulkanDevice>(this, CurGpu);
		if (NewDevice->QueryGPU()) {
			Device = NewDevice;
			break;
		}
	}

	if (Device == nullptr) {
		std::cerr << "No devices found!" << std::endl;
	}
	assert(Device);
	Device->InitGPU();

}



void VulkanRHI::SetupDebugLayerCallback(){
#if VULKAN_HAS_DEBUGGING_ENABLED
	PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessengerEXT = 
		(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Instance, "vkCreateDebugUtilsMessengerEXT");
	if (CreateDebugUtilsMessengerEXT)
	{
		VkDebugUtilsMessengerCreateInfoEXT CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

		CreateInfo.messageSeverity =
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;

		CreateInfo.messageType =
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		CreateInfo.pfnUserCallback = (PFN_vkDebugUtilsMessengerCallbackEXT)(void*)DebugUtilsCallback;
		VkResult Result = (*CreateDebugUtilsMessengerEXT)(Instance, &CreateInfo, nullptr, &Messenger);

		if (Result != VK_SUCCESS) {
			std::cerr << "SetupDebugLayerCallback Error" << std::endl;
		}
	}
#endif
}

VkBool32 VulkanRHI::DebugUtilsCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT MsgSeverity,
	VkDebugUtilsMessageTypeFlagsEXT MsgType,
	const VkDebugUtilsMessengerCallbackDataEXT* CallbackData,
	void* UserData)
{
	std::cerr << "validation layer: " << CallbackData->pMessage << std::endl;
	return VK_FALSE;
}


void VulkanRHI::RemoveDebugLayerCallback(){
#if VULKAN_HAS_DEBUGGING_ENABLED
	if (Messenger != VK_NULL_HANDLE)
	{
		PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessengerEXT = 
			(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Instance, "vkDestroyDebugUtilsMessengerEXT");

		if (DestroyDebugUtilsMessengerEXT)
		{
			(*DestroyDebugUtilsMessengerEXT)(Instance, Messenger, nullptr);
		}
	}
#endif
}

VkShaderModule VulkanRHI::LoadSpvShader(const std::string& FilePath) noexcept{
	std::ifstream is(FilePath, std::ios::binary | std::ios::in | std::ios::ate);
	assert(is.is_open());
	size_t shaderSize;
	shaderSize = is.tellg();
	std::unique_ptr<char> ShaderData(new char[shaderSize]);
	is.seekg(0, std::ios::beg);
	is.read(ShaderData.get(), shaderSize);
	is.close();
	
	// Create a new shader module that will be used for pipeline creation
	VkShaderModuleCreateInfo moduleCreateInfo{};
	moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	moduleCreateInfo.codeSize = shaderSize;
	moduleCreateInfo.pCode = (uint32_t*)ShaderData.get();
	VkShaderModule shaderModule;
	assert(vkCreateShaderModule(Device->GetLogicDevice(), &moduleCreateInfo, NULL, &shaderModule) == VK_SUCCESS);
	return shaderModule;
}


