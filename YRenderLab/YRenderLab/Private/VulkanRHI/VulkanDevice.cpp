#include <Public/VulkanRHI/VulkanDevice.h>
#include <Public/VulkanRHI/VulkanRHI.h>


YVulkanDevice::YVulkanDevice(VulkanRHI* InRHI, VkPhysicalDevice InGpu)
	: Device(VK_NULL_HANDLE)
	, Gpu(InGpu)
	, VkRHI(InRHI)
{

}

YVulkanDevice::~YVulkanDevice(){

}

bool YVulkanDevice::QueryGPU()
{
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(Gpu, &deviceProperties);

	bool bIsDiscrete = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
	bool bIsNvida = deviceProperties.vendorID == static_cast<uint32_t>(VenderID::Nvidia);

	return bIsDiscrete && bIsNvida;
}
