#include <Public/VulkanRHI/VulkanDevice.h>
#include <Public/VulkanRHI/VulkanRHI.h>
#include <Public/VulkanRHI/VulkanQueue.h>


VulkanDevice::VulkanDevice(VulkanRHI* InRHI, VkPhysicalDevice InGpu)
	: VkRHI(InRHI)
	, LogicalDevice(VK_NULL_HANDLE)
	, PhysicalDevice(InGpu)
	, GfxQueue(nullptr)
	, ComputeQueue(nullptr)
	, TransferQueue(nullptr)
{
	std::memset(&PhysicalFeatures, 0, sizeof(VkPhysicalDeviceFeatures));
}

VulkanDevice::~VulkanDevice() {

	if (LogicalDevice != VK_NULL_HANDLE){

		vkDestroyCommandPool(LogicalDevice, CommandlBufferPool[CommandPoolType::GfxPool], nullptr);
		vkDestroyCommandPool(LogicalDevice, CommandlBufferPool[CommandPoolType::ComputePool], nullptr);
		vkDestroyCommandPool(LogicalDevice, CommandlBufferPool[CommandPoolType::TransferPool], nullptr);

		delete TransferQueue;
		delete ComputeQueue;
		delete GfxQueue;

		vkDestroyDevice(LogicalDevice, nullptr);
		LogicalDevice = VK_NULL_HANDLE;
	}
}

bool VulkanDevice::QueryGPU(){

	VulkanDevice::GetDeviceExtensionsAndLayers(PhysicalDevice, DeviceExtensions, DeviceLayers);
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(PhysicalDevice, &deviceProperties);

	//独立显卡
	bool bIsDiscrete = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
	bool bIsNvida = deviceProperties.vendorID == static_cast<uint32_t>(VenderID::Nvidia);

	uint32_t QueueCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &QueueCount, nullptr);
	assert(QueueCount >= 1);
	QueueFamilyProps.resize(QueueCount);
	vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &QueueCount, QueueFamilyProps.data());
	return bIsDiscrete && bIsNvida;
}

void VulkanDevice::InitGPU() {

	// Query features
	vkGetPhysicalDeviceFeatures(PhysicalDevice, &PhysicalFeatures);
	vkGetPhysicalDeviceMemoryProperties(PhysicalDevice, &MemoryProperties);
	CreateLogicDevice();

	CreateAllCommandQueue();
}


void VulkanDevice::CreateLogicDevice() {
	assert(LogicalDevice == VK_NULL_HANDLE);
	std::vector<VkDeviceQueueCreateInfo> QueueFamilyInfos;
	int32_t GfxQueueFamilyIndex = -1;
	int32_t ComputeQueueFamilyIndex = -1;
	int32_t TransferQueueFamilyIndex = -1;
	printf("Found %lld Queue Families\n", QueueFamilyProps.size());

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
			printf("Skipping unnecessary Queue Family %d: %d queues\n", FamilyIndex, CurrProps.queueCount);
			continue;
		}

		VkDeviceQueueCreateInfo CurrQueue = {};
		CurrQueue.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		CurrQueue.queueFamilyIndex = FamilyIndex;
		CurrQueue.queueCount = CurrProps.queueCount; //一种类型的队列数量不固定
		NumPriorities += CurrQueue.queueCount; //每一个队列都要有一个对应的float数据指定优先级,所以priorities的数量为Queue的总数

		QueueFamilyInfos.emplace_back(CurrQueue);
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
	VkResult Result = vkCreateDevice(PhysicalDevice, &DeviceInfo, nullptr, &LogicalDevice);

	if (Result != VK_SUCCESS) {
		std::cerr << "Vulkan device creation failed" << std::endl;
	}

	//Gfx Queue
	if (GfxQueueFamilyIndex == -1) {
		std::cerr << "Queue has Error" << std::endl;
		assert(GfxQueueFamilyIndex == -1);
	}
	GfxQueue = new VulkanQueue(this, GfxQueueFamilyIndex);

	//Compute Queue
	if (ComputeQueueFamilyIndex == -1) {
		std::cerr << "Queue has Error" << std::endl;
		assert(ComputeQueueFamilyIndex == -1);
	}
	ComputeQueue = new VulkanQueue(this, ComputeQueueFamilyIndex);

	//Transfer Queue
	if (TransferQueueFamilyIndex == -1) {
		std::cerr << "Queue has Error" << std::endl;
		assert(TransferQueueFamilyIndex == -1);
	}
	TransferQueue = new VulkanQueue(this, TransferQueueFamilyIndex);
}

void VulkanDevice::CreateAllCommandQueue()
{
	//Gfx Command pool
	VkCommandPoolCreateInfo cmdPoolInfo = {};
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = GfxQueue->GetFamilyIndex();
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	assert(vkCreateCommandPool(LogicalDevice, &cmdPoolInfo, nullptr, &CommandlBufferPool[CommandPoolType::GfxPool]) == VK_SUCCESS);

	//Compute Command pool
	cmdPoolInfo.queueFamilyIndex = ComputeQueue->GetFamilyIndex();
	assert(vkCreateCommandPool(LogicalDevice, &cmdPoolInfo, nullptr, &CommandlBufferPool[CommandPoolType::ComputePool]) == VK_SUCCESS);

	//Transfer Command pool
	cmdPoolInfo.queueFamilyIndex = TransferQueue->GetFamilyIndex();
	assert(vkCreateCommandPool(LogicalDevice, &cmdPoolInfo, nullptr, &CommandlBufferPool[CommandPoolType::TransferPool]) == VK_SUCCESS);
}

VulkanQueue* VulkanDevice::SetupPresentQueue(VkSurfaceKHR Surface){
	VkBool32 bSupportsPresent = VK_FALSE;
	const auto FamilyIndex = GfxQueue->GetFamilyIndex();
	vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, FamilyIndex, Surface, &bSupportsPresent);
	assert(bSupportsPresent);
	//just gfxQueue
	return  GfxQueue;
}

void VulkanDevice::AllocateCommandBuffer(CommandPoolType PoolType, uint32_t CmdCount, VkCommandBuffer* CmdBuffer){

	VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
	cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufAllocateInfo.commandPool = CommandlBufferPool[PoolType];
	cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdBufAllocateInfo.commandBufferCount = 1;
	assert(vkAllocateCommandBuffers(LogicalDevice, &cmdBufAllocateInfo, CmdBuffer) == VK_SUCCESS);
}

VkCommandPool VulkanDevice::GetCommandBufferPool(CommandPoolType PoolType){
	return CommandlBufferPool[PoolType];
}
