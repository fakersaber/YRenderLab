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
//#TODO: 删除不同接口 统一RHI
#if SOFT_RENDER
	this->_RenderWindow = Win32Window::GetInstance();
#elif OPENGL_RENDER
	this->pGLInstance = New<GlfwWindow>();
#elif VULKAN_RENDER
	RenderRHI = std::make_shared<VulkanRHI>();
	SurfaceRenderWindow = std::make_shared<Window_GLFW>();
#endif 
}


void YCore::Init(const int width, const int height) {
	//1.初始化顺序固定
	//2.尽可能的资源不耦合，各自管理创建的资源
	//3.若引用其他模块，使用强引用
	RenderRHI->Init();
	SurfaceRenderWindow->Init(width, height, RenderRHI);
}

void YCore::Shutdown(){
	//Shutdown用于释放引用其他模块的资源
	RenderRHI->Shutdown(); 
	SurfaceRenderWindow->Shutdown();
}

void YCore::Tick() {
	SurfaceRenderWindow->Tick();
}


YCore::~YCore() {

}
