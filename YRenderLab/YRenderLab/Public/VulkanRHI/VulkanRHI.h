#ifndef _YRENDER_VKRHI_VulkanRHI_H_
#define _YRENDER_VKRHI_VulkanRHI_H_

#include <Public/RHI/RHI.h>

#define GLFW_INCLUDE_VULKAN
#include <Public/3rdPart/GLFW/glfw3.h>
#include <vector>

class YVulkanDevice;

class VulkanRHI : public RHI {

public:
	VulkanRHI();
	virtual void Init() override;

protected:
	void CreateInstance();
	void GetInstanceExtensionsAndLayers(std::vector<const char*>& Entensions, std::vector<const char*>& Layers);

protected:
	VkInstance Instance;
	YVulkanDevice* Devices;

	std::vector<const char*> InstanceExtensions;
	std::vector<const char*> InstanceLayers;
};



#endif