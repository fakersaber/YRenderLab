#include <Public/VulkanRHI/VulkanQueue.h>
#include <Public/VulkanRHI/VulkanDevice.h>

VulkanQueue::VulkanQueue(VulkanDevice * InDevice, uint32_t InFamilyIndex)
	: Device(InDevice)
	, Queue(VK_NULL_HANDLE)
	, FamilyIndex(InFamilyIndex)
	, QueueIndex(0)
{
	vkGetDeviceQueue(Device->GetLogicDevice(), FamilyIndex, QueueIndex, &Queue);
}


VulkanQueue::~VulkanQueue()
{

}