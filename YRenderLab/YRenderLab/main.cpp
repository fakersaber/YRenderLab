#include <Public/YCore.h>

#include <glad/glad.h>
#include <glfw3.h>



//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	glViewport(0, 0, width, height);
//}



int main() {
	//glfwInit();
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//GLFWwindow* window = glfwCreateWindow(800, 600, "YRender", nullptr, nullptr);
	//if (window == nullptr)
	//{
	//	std::cout << "Failed to create GLFW window" << std::endl;
	//	glfwTerminate();
	//	return -1;
	//}
	//glfwMakeContextCurrent(window);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	//if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	//{
	//	std::cout << "Failed to initialize GLAD" << std::endl;
	//	return -1;
	//}
	//glViewport(0, 0, 800, 600);
	
	auto YEngine = YRender::YCore::GetCore();
	if (YEngine->Initial(800, 600)) {
		YEngine->Run();
	}

}