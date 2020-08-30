#ifndef _YRENDER_VKRHI_VulkanRHI_H_
#define _YRENDER_VKRHI_VulkanRHI_H_

#include <Public/YRenderLabConfig.h>
#include <Public/RHI/RHI.h>
#include <Public/RHI/IPipeline.h>
#include <vulkan/vulkan.h>
#include <Public/VulkanRHI/VulkanConfig.h>

#if USE_WINDOWS_PLATFORM
#include <Public/VulkanRHI/VulkanWindowsPlatform.h>
#endif
#include <vector>


class VulkanDevice;
class VulkanPipeline;

enum class VenderID : uint32_t {
	Nvidia = 0x10DE,
	AMD = 0x1002
};


class VulkanRHI final : public RHI {
public:
	VulkanRHI();
	virtual ~VulkanRHI();

	//RHI Function
	virtual void Init() final override;
	virtual void Shutdown() final override;
	virtual void SetupFormat() final override;
	virtual IPipeline* RHICreateRenderPipeline(void* WindowHandle, uint32_t SizeX, uint32_t SizeY, EPixelFormat PreferredPixelFormat) final override;

	void SetComponentMapping(EPixelFormat UEFormat, VkComponentSwizzle r, VkComponentSwizzle g, VkComponentSwizzle b, VkComponentSwizzle a);
	VkComponentMapping GetComponentMapping(const EPixelFormat UEFormat) const;
	inline VkInstance GetInstance() const { return Instance; }
	inline VulkanDevice* GetDevice() const { return Device.get(); }

	//--------------------------Static Func-----------------------------//
	static unsigned int SRGBMapping(EPixelFormat UEFormat);
	static VkBool32 DebugUtilsCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT MsgSeverity, 
		VkDebugUtilsMessageTypeFlagsEXT MsgType,
		const VkDebugUtilsMessengerCallbackDataEXT* CallbackData, 
		void* UserData);


protected:
	void CreateInstance();
	void SelectAndInitDevice();

	void SetupDebugLayerCallback();
	void RemoveDebugLayerCallback();

	static void GetInstanceLayersAndExtensions(std::vector<const char*>& Entensions, std::vector<const char*>& Layers);



protected:
	//[Resource management]
	VkInstance Instance;
	std::shared_ptr<VulkanDevice> Device; //因为创建Devices时会创建多个，但是只会使用一个，要保证在函数作用域释放
	std::vector<const char*> InstanceExtensions;
	std::vector<const char*> InstanceLayers;
	VkComponentMapping PixelFormatComponentMapping[static_cast<unsigned int>(EPixelFormat::PF_MAX)];	//只有Vulkan才会有，所以不与Platform的PixelData放在一起,实际也可以通过重载PixelFormatInfo来达成

#if VULKAN_HAS_DEBUGGING_ENABLED
	VkDebugUtilsMessengerEXT Messenger = VK_NULL_HANDLE;
#endif
};



#endif