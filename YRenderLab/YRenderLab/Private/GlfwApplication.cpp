#include <Public/GlfwApplication.h>
#include <Public/YRenderLabConfig.h>
#include <Public/Scene/RenderScene.h>
#if OPENGL_RENDER
#include <Public/OpenGLRHI/GlfwWindow.h>
#endif

#if VULKAN_RENDER
#include <Public/VulkanRHI/VulkanRHI.h>
#define GLFW_INCLUDE_VULKAN
#endif

#include <Public/3rdPart/GLFW/glfw3.h>

#if USE_WINDOWS_PLATFORM
#define GLFW_EXPOSE_NATIVE_WIN32
#include <Public/3rdPart/GLFW/glfw3native.h>
#endif



void GlfwApplication::Init(const int width, const int height) {
	//--------------Init RHI--------------//
	RenderRHI = new VulkanRHI();
	RenderRHI->Init();

	//--------------Init Window--------------//
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(width, height, "YRender", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	//it’s the Targa bitmap format (.tga). It stores the pixel data in BGRA format.so Use BGRA?
	RenderPipeline = RenderRHI->RHICreateRenderPipeline(glfwGetWin32Window(window), width, height, PF_B8G8R8A8);

	World = new RenderScene();
}

void GlfwApplication::Shutdown() {
	delete RenderPipeline;
	delete RenderRHI;
	delete World;
	glfwTerminate();
}

void GlfwApplication::Update(){
	//处理注册的回调函数
	glfwPollEvents();
}

void GlfwApplication::Render(){
	RenderPipeline->BeginFrame(World);
	RenderPipeline->Render();
	RenderPipeline->EndFrame();
}

void GlfwApplication::Tick() {
	while (!glfwWindowShouldClose(window)) {
		Update();
		Render();
	}
}

