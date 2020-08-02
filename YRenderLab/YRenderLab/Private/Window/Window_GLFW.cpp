#include <Public/YCore.h>
#include <Public/Window/Window_GLFW.h>

#if VULKAN_RENDER
#define GLFW_INCLUDE_VULKAN
#endif // 
#include <Public/3rdPart/GLFW/glfw3.h>

#if USE_WINDOWS_PLATFORM
#define GLFW_EXPOSE_NATIVE_WIN32
#include <Public/3rdPart/GLFW/glfw3native.h>
#endif

Window_GLFW::Window_GLFW(){

}

Window_GLFW::~Window_GLFW(){

}

void Window_GLFW::Init(const int width, const int height, const std::shared_ptr<RHI>& RHIResource){
	glfwInit();

#if OPENGL_RENDER
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

#if VULKAN_RENDER
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif // 

	window = glfwCreateWindow(width, height, "YRender", NULL, NULL);

	if (window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	//glfwGetWin32Window()
}

void Window_GLFW::Shutdown(){
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window_GLFW::Tick(){
	while (!glfwWindowShouldClose(window)) {

		ViewportRHI->Draw();

#if OPENGL_RENDER
		double beginTime = glfwGetTime();
		ProcessInput(window);
		beginTime = glfwGetTime();
		RenderRaster->Draw();
		deltaTime = static_cast<float>(glfwGetTime() - beginTime);
		glfwSwapBuffers(window);
#endif
		glfwPollEvents();
	}
}
