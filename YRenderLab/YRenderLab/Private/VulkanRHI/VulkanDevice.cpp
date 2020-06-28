#include <Public/VulkanRHI/VulkanDevice.h>
#include <Public/VulkanRHI/VulkanRHI.h>


VulkanDevice::VulkanDevice(VulkanRHI* InRHI, VkPhysicalDevice InGpu)
	: Device(VK_NULL_HANDLE)
	, Gpu(InGpu)
	, VkRHI(InRHI)
{
	std::memset(&PhysicalFeatures, 0, sizeof(VkPhysicalDeviceFeatures));
}

VulkanDevice::~VulkanDevice() {
	if (Device != VK_NULL_HANDLE){
		vkDestroyDevice(Device, nullptr);
		Device = VK_NULL_HANDLE;
	}
}

bool VulkanDevice::QueryGPU()
{
	VulkanDevice::GetDeviceExtensionsAndLayers(Gpu, DeviceExtensions, DeviceLayers);

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(Gpu, &deviceProperties);
	bool bIsDiscrete = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
	bool bIsNvida = deviceProperties.vendorID == static_cast<uint32_t>(VenderID::Nvidia);

	uint32_t QueueCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(Gpu, &QueueCount, nullptr);
	assert(QueueCount >= 1);
	QueueFamilyProps.resize(QueueCount);
	vkGetPhysicalDeviceQueueFamilyProperties(Gpu, &QueueCount, QueueFamilyProps.data());

	return bIsDiscrete && bIsNvida;
}

void VulkanDevice::InitGPU() {

	// Query features
	vkGetPhysicalDeviceFeatures(Gpu, &PhysicalFeatures);

	CreateDevice();
}


void VulkanDevice::CreateDevice() {
	assert(Device == VK_NULL_HANDLE);

	std::vector<VkDeviceQueueCreateInfo> QueueFamilyInfos;
	int32_t GfxQueueFamilyIndex = -1;
	int32_t ComputeQueueFamilyIndex = -1;
	int32_t TransferQueueFamilyIndex = -1;
	printf("Found %lld Queue Families", QueueFamilyProps.size());

	uint32_t NumPriorities = 0;

	for (auto FamilyIndex = 0; FamilyIndex < QueueFamilyProps.size(); ++FamilyIndex) {
		const VkQueueFamilyProperties& CurrProps = QueueFamilyProps[FamilyIndex];
		bool bIsValidQueue = false;

		//Graph pipeline
		if ((CurrProps.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT && GfxQueueFamilyIndex == -1) {
			GfxQueueFamilyIndex = FamilyIndex;
			bIsValidQueue = true;
		}

		//compoute pipeline
		if ((CurrProps.queueFlags & VK_QUEUE_COMPUTE_BIT) == VK_QUEUE_COMPUTE_BIT && ComputeQueueFamilyIndex == -1) {
			//当前队列未被初始化时才赋值
			if (!bIsValidQueue) {
				ComputeQueueFamilyIndex = FamilyIndex;
				bIsValidQueue = true;
			}
		}

		//transfer 回读queue
		if ((CurrProps.queueFlags & VK_QUEUE_TRANSFER_BIT) == VK_QUEUE_TRANSFER_BIT) {
			//当前队列未被初始化时才赋值
			if (!bIsValidQueue) {
				TransferQueueFamilyIndex = FamilyIndex;
				bIsValidQueue = true;
			}
		}

		if (!bIsValidQueue) {
			printf("Skipping unnecessary Queue Family %d: %d queues", FamilyIndex, CurrProps.queueCount);
			continue;
		}

		VkDeviceQueueCreateInfo CurrQueue = {};
		CurrQueue.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		CurrQueue.queueFamilyIndex = FamilyIndex;
		CurrQueue.queueCount = CurrProps.queueCount; //一种类型的队列数量不固定
		NumPriorities += CurrQueue.queueCount; //每一个队列都要有一个对应的float数据指定优先级,所以priorities的数量为Queue的总数
	}

	std::vector<float> QueuePriorities;
	QueuePriorities.resize(NumPriorities);
	float* CurrentPriority = QueuePriorities.data();

	for (auto& CurrQueue : QueueFamilyInfos) {
		CurrQueue.pQueuePriorities = CurrentPriority;
		const VkQueueFamilyProperties& CurrProps = QueueFamilyProps[CurrQueue.queueFamilyIndex];

		//可对每个Priorities单独赋值，不批量赋值
		for (int32_t QueueIndex = 0; QueueIndex < static_cast<int32_t>(CurrProps.queueCount); ++QueueIndex) {
			*CurrentPriority++ = 1.f;
		}
	}


	VkDeviceCreateInfo DeviceInfo = {};
	DeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	DeviceInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions.size());
	DeviceInfo.ppEnabledExtensionNames = DeviceExtensions.data();
	DeviceInfo.enabledLayerCount = static_cast<uint32_t>(DeviceLayers.size());
	DeviceInfo.ppEnabledLayerNames = (DeviceInfo.enabledLayerCount > 0) ? DeviceLayers.data() : nullptr;

	DeviceInfo.queueCreateInfoCount = static_cast<uint32_t>(QueueFamilyInfos.size());
	DeviceInfo.pQueueCreateInfos = QueueFamilyInfos.data();

	DeviceInfo.pEnabledFeatures = &PhysicalFeatures;


	// Create the device

	//the third param is allocation pointer to store the logical device handle in
	VkResult Result = vkCreateDevice(Gpu, &DeviceInfo, nullptr, &Device);

	if (Result != VK_SUCCESS) {
		std::cerr << "Vulkan device creation failed" << std::endl;
	}

	//Gfx Queue
	if (GfxQueueFamilyIndex == -1) {
		std::cerr << "Queue has Error" << std::endl;
	}
	vkGetDeviceQueue(this->Device, GfxQueueFamilyIndex, 0, &GfxQueue);

	//Compute Queue
	if (ComputeQueueFamilyIndex == -1) {
		std::cerr << "Queue has Error" << std::endl;
	}
	vkGetDeviceQueue(this->Device, ComputeQueueFamilyIndex, 0, &ComputeQueue);

	//Transfer Queue
	if (TransferQueueFamilyIndex == -1) {
		std::cerr << "Queue has Error" << std::endl;
	}
	vkGetDeviceQueue(this->Device, TransferQueueFamilyIndex, 0, &TransferQueue);


}

