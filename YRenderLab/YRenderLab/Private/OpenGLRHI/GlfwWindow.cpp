#include <Public/OpenGLRHI/GlfwWindow.h>



namespace YRender {
	GlfwWindow * GlfwWindow::GetInstance(){
		static GlfwWindow instance;
		return &instance;
	}

	bool GlfwWindow::Initial(const int width, const int height) {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		window = glfwCreateWindow(width, height, "YRender", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}

		this->_RenderClass = std::make_unique<GlRenderClass>(window);
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


		if (!_RenderClass->Initial(width, height)) {
			return false;
		}
		return true;
	}

	void GlfwWindow::Run(){
		while (!glfwWindowShouldClose(window)) {
			ProcessInput(window);

			//RenderClass Update
			{
				_RenderClass->Tick();
				_RenderClass->Render();
			}

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}



	GlfwWindow::GlfwWindow(){

	}



	GlfwWindow::~GlfwWindow(){

	}


	void GlfwWindow::ProcessInput(GLFWwindow* window) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	void GlfwWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}
}
