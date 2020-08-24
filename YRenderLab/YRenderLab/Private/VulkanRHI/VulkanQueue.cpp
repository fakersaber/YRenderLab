#include <Public/VulkanRHI/VulkanQueue.h>
#include <Public/VulkanRHI/VulkanDevice.h>

VulkanQueue::VulkanQueue(VulkanDevice * InDevice, uint32_t InFamilyIndex)
	: Queue(VK_NULL_HANDLE)
	, FamilyIndex(InFamilyIndex)
	, QueueIndex(0)
	, Device(InDevice)
{
	vkGetDeviceQueue(Device->GetLogicDevice(), FamilyIndex, QueueIndex, &Queue);
}


VulkanQueue::~VulkanQueue()
{

}