#include <Public/OpenGLRHI/GlfwWindow.h>



namespace YRender {
	GlfwWindow* GlfwWindow::GetInstance(){
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
		lastX = width * 0.2f;
		lastY = height * 0.2f;
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, GlfwWindow::framebuffer_size_callback);
		glfwSetCursorPosCallback(window, GlfwWindow::mouse_callback);
		glfwSetScrollCallback(window, GlfwWindow::scroll_callback);



		if (!_RenderClass->Initial(width, height)) {
			return false;
		}
		return true;
	}

	void GlfwWindow::Run(){
		while (!glfwWindowShouldClose(window)) {
			double beginTime = glfwGetTime();
			ProcessInput(window);
			//RenderClass Update
			{
				beginTime = glfwGetTime();
				_RenderClass->Tick();
				_RenderClass->Render();
				deltaTime = glfwGetTime() - beginTime;
			}
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}



	GlfwWindow::GlfwWindow()
		:firstFlag(true)
	{

	}


	GlfwWindow::~GlfwWindow(){

	}


	void GlfwWindow::ProcessInput(GLFWwindow* window) {

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			GlfwWindow::GetInstance()->_RenderClass->GetCamera().ProcessKeyboard(Camera::ENUM_Movement::MOVE_FORWARD, GlfwWindow::GetInstance()->GetDeltaTime());
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			GlfwWindow::GetInstance()->_RenderClass->GetCamera().ProcessKeyboard(Camera::ENUM_Movement::MOVE_BACKWARD, GlfwWindow::GetInstance()->GetDeltaTime());
		else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			GlfwWindow::GetInstance()->_RenderClass->GetCamera().ProcessKeyboard(Camera::ENUM_Movement::MOVE_LEFT, GlfwWindow::GetInstance()->GetDeltaTime());
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			GlfwWindow::GetInstance()->_RenderClass->GetCamera().ProcessKeyboard(Camera::ENUM_Movement::MOVE_RIGHT, GlfwWindow::GetInstance()->GetDeltaTime());
	}

	void GlfwWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}

	void GlfwWindow::mouse_callback(GLFWwindow* window, double xpos, double ypos){

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			float xoffset = 0.f;
			float yoffset = 0.f;
			if (!GlfwWindow::GetInstance()->firstFlag) {
				//不是第一次按住鼠标右键才计算差值
				xoffset = GlfwWindow::GetInstance()->lastX - xpos;
				yoffset = GlfwWindow::GetInstance()->lastY - ypos;
			}
			GlfwWindow::GetInstance()->lastX = xpos;
			GlfwWindow::GetInstance()->lastY = ypos;
			GlfwWindow::GetInstance()->firstFlag = false;
			GlfwWindow::GetInstance()->_RenderClass->GetCamera().ProcessMouseMovement(xoffset, yoffset);
		}
		else {
			GlfwWindow::GetInstance()->firstFlag = true;
		}
	}

	void GlfwWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	}

	void GlfwWindow::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){

	}
}
