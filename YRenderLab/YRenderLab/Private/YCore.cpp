#include <Public/YCore.h>
#if USE_GLFWWINDOW
#include <Public/Window/Window_GLFW.h>
#endif

#if OPENGL_RENDER
#include <Public/OpenGLRHI/GlfwWindow.h>
#endif

#if VULKAN_RENDER
#include <Public/VulkanRHI/VulkanRHI.h>
#endif


YCore* YCore::GetCore() {
	static YCore instance;
	return &instance;
}


YCore::YCore() {
#if SOFT_RENDER
	this->_RenderWindow = Win32Window::GetInstance();
#elif OPENGL_RENDER
	this->pGLInstance = New<GlfwWindow>();
#elif VULKAN_RENDER
	SurfaceRenderWindow = new Window_GLFW();
	RenderRHI = new VulkanRHI();
#endif 
}

YCore::~YCore() {
	delete SurfaceRenderWindow;
	delete RenderRHI;
}


bool YCore::Initial(const int width, const int height) {

	auto Result = SurfaceRenderWindow->Initial(width, height);

	assert(Result);

	RenderRHI->Init();

	return true;
}

void YCore::Shutdown(){
	RenderRHI->Shutdown();
	SurfaceRenderWindow->Shutdown();
}

void YCore::Run() {
	SurfaceRenderWindow->Run();
}


