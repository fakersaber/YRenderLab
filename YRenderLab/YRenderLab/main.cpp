//#include <Public/YCore.h>

#include <iostream>

//#include <Public/Basic/Image/Image.h>


//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>

//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	glViewport(0, 0, width, height);
//}

class Test {
public:
	Test() {
		std::cout << "test" << std::endl;
	}

	Test& operator=(const Test& rhs) {
		std::cout << "test2" << std::endl;
		return *this;
	}


	Test(const Test& rhs) {
		*this = rhs;
		std::cout << "test3" << std::endl;
	}
};

int main() {
	//Assimp::Importer importer;
	//const aiScene *scene = importer.ReadFile("test", aiProcess_Triangulate | aiProcess_FlipUVs);

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

	//YRender::Image a("C:\\Users\\Administrator\\Desktop\\first.png");
	//YRender::RGBAf result = a.SampleNearest(0.5f,0.5f);
	//auto test = result.RBGA2UINT();

	Test a = Test();
	std::cout << &a << std::endl;
	//auto YEngine = YRender::YCore::GetCore();
	//if (YEngine->Initial(800, 600)) {
	//	YEngine->Run();
	//}
}