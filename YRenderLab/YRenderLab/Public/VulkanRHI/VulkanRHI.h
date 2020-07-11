#ifndef _YRENDER_VKRHI_VulkanRHI_H_
#define _YRENDER_VKRHI_VulkanRHI_H_

#include <Public/YCore.h>
#include <Public/RHI/RHI.h>

#include <vulkan/vulkan.h>
#include <Public/VulkanRHI/VulkanConfig.h>
#if USE_WINDOWS_PLATFORM
#include <Public/VulkanRHI/VulkanWindowsPlatform.h>
#endif

#include <vector>

class VulkanDevice;
class VulkanViewPort;

enum class VenderID : uint32_t {
	Nvidia = 0x10DE,
	AMD = 0x1002
};


class VulkanRHI final : public RHI {
public:
	VulkanRHI();
	virtual ~VulkanRHI();
	virtual void Init() final override;
	virtual void Shutdown() final override;
	virtual void SetupFormat() final override;


	static VkBool32 DebugUtilsCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT MsgSeverity, 
		VkDebugUtilsMessageTypeFlagsEXT MsgType,
		const VkDebugUtilsMessengerCallbackDataEXT* CallbackData, 
		void* UserData);

	inline VkInstance GetInstance() const{
		return Instance;
	}

	inline VulkanDevice* GetDevice() const {
		return Device.get();
	}

protected:
	void CreateInstance();
	void SelectAndInitDevice();

	void SetupDebugLayerCallback();
	void RemoveDebugLayerCallback();

	static void GetInstanceLayersAndExtensions(std::vector<const char*>& Entensions, std::vector<const char*>& Layers);



protected:
	VkInstance Instance;

	std::shared_ptr<VulkanDevice> Device;


#if VULKAN_SUPPORTS_DEBUG_UTILS
	VkDebugUtilsMessengerEXT Messenger = VK_NULL_HANDLE;
#endif

	std::vector<const char*> InstanceExtensions;
	std::vector<const char*> InstanceLayers;
};



#endif