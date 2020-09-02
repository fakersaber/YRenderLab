
template<bool bWriteBuffer>
void VulkanBufferResource::CreateBuffer(
	VulkanDevice* Device,
	VkBufferUsageFlags BufferUsage,
	VkMemoryPropertyFlags MemoryType,
	uint32_t BufferSize,
	void* WriteBuffer
)
{
	assert(BufferSize > 0);
	auto DeviceRef = Device->GetLogicDevice();
	VkMemoryAllocateInfo memAlloc = {};
	memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	VkMemoryRequirements memReqs;
	VkBufferCreateInfo indexbufferInfo = {};
	indexbufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	indexbufferInfo.size = BufferSize;
	indexbufferInfo.usage = BufferUsage;

	//Create Memory
	assert(vkCreateBuffer(DeviceRef, &indexbufferInfo, nullptr, &ResourceBuffer) == VK_SUCCESS);
	vkGetBufferMemoryRequirements(DeviceRef, ResourceBuffer, &memReqs);
	memAlloc.allocationSize = memReqs.size;
	memAlloc.memoryTypeIndex = Device->GetMemoryTypeIndex(memReqs.memoryTypeBits, MemoryType);
	assert(vkAllocateMemory(DeviceRef, &memAlloc, nullptr, &ResourceBufferMemory) == VK_SUCCESS);

	//Write data and bind
	if (bWriteBuffer) {
		void* MappingMemory = nullptr;
		assert(vkMapMemory(DeviceRef, ResourceBufferMemory, 0, BufferSize, 0, &MappingMemory) == VK_SUCCESS);
		std::memcpy(MappingMemory, WriteBuffer, BufferSize);
		vkUnmapMemory(DeviceRef, ResourceBufferMemory);
	}

	assert(vkBindBufferMemory(DeviceRef, ResourceBuffer, ResourceBufferMemory, 0) == VK_SUCCESS);
}

template<bool bWriteBuffer>
void VulkanIndexBufferResource::CreateBuffer(
	VulkanDevice* Device,
	VkBufferUsageFlags BufferUsage,
	VkMemoryPropertyFlags MemoryType,
	uint32_t BufferSize,
	void* WriteBuffer
) 
{
	VulkanBufferResource::CreateBuffer<bWriteBuffer>(Device, BufferUsage, MemoryType, BufferSize, WriteBuffer);
	IndexCount = BufferSize >> 2ul;  //BufferSize / sizeof(uint32_t)
}


