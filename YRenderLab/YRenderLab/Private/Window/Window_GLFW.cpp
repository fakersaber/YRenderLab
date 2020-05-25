#include <Public/Window/Window_GLFW.h>
#include <Public/YCore.h>

#if VULKAN_RENDER
#define GLFW_INCLUDE_VULKAN
#endif // 
#include <Public/3rdPart/GLFW/glfw3.h>


Window_GLFW::Window_GLFW(){

}

Window_GLFW::~Window_GLFW(){

}

bool Window_GLFW::Initial(const int width, const int height){
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
		return false;
	}

	return true;
}

void Window_GLFW::Shutdown(){
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window_GLFW::Run(){
	while (!glfwWindowShouldClose(window)) {

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
