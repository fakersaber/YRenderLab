#include <Public/OpenGLRHI/GlfwWindow.h>
#include <Public/Scene/AssimpLoader.h>
#include <Public/Scene/Scene.h>
#include <Public/OpenGLRHI/GLAD/glad/glad.h>

namespace YRender {
	GlfwWindow* GlfwWindow::GetInstance(){
		static GlfwWindow instance;
		return &instance;
	}

	bool GlfwWindow::Initial(const int width, const int height) {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		window = glfwCreateWindow(width, height, "YRender", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}

		lastX = width * 0.5f;
		lastY = height * 0.5f;
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, GlfwWindow::framebuffer_size_callback);
		glfwSetCursorPosCallback(window, GlfwWindow::mouse_callback);
		//glfwSetScrollCallback(window, GlfwWindow::scroll_callback);

		//初始化glad
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return false;
		}

		//创建场景、管线、相机
		auto Root = YRender::AssimpLoader::Load("C:/Users/Administrator/Desktop/nanosuit/nanosuit.obj");
		auto Scene = YRender::New<YRender::Scene>(Root);
		MainCamera = YRender::New<Camera>();
		ForwardPipline = YRender::New<ForwardRaster>(Scene, MainCamera);

		ForwardPipline->Initial();
		MainCamera->Initial(width, height);

		return true;
	}

	void GlfwWindow::Run(){
		while (!glfwWindowShouldClose(window)) {
			double beginTime = glfwGetTime();
			ProcessInput(window);
			beginTime = glfwGetTime();

			ForwardPipline->Draw();

			deltaTime = static_cast<float>(glfwGetTime() - beginTime);
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
			GlfwWindow::GetInstance()->MainCamera->ProcessKeyboard(Camera::ENUM_Movement::MOVE_FORWARD, GlfwWindow::GetInstance()->GetDeltaTime());
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			GlfwWindow::GetInstance()->MainCamera->ProcessKeyboard(Camera::ENUM_Movement::MOVE_BACKWARD, GlfwWindow::GetInstance()->GetDeltaTime());
		else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			GlfwWindow::GetInstance()->MainCamera->ProcessKeyboard(Camera::ENUM_Movement::MOVE_LEFT, GlfwWindow::GetInstance()->GetDeltaTime());
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			GlfwWindow::GetInstance()->MainCamera->ProcessKeyboard(Camera::ENUM_Movement::MOVE_RIGHT, GlfwWindow::GetInstance()->GetDeltaTime());
	}

	void GlfwWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
		GlfwWindow::GetInstance()->MainCamera->SetWH(width,height);
	}

	void GlfwWindow::mouse_callback(GLFWwindow* window, double xpos, double ypos){

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			float xoffset = 0.f;
			float yoffset = 0.f;
			if (!GlfwWindow::GetInstance()->firstFlag) {
				//不是第一次按住鼠标右键才计算差值
				xoffset = GlfwWindow::GetInstance()->lastX - static_cast<float>(xpos);
				yoffset = GlfwWindow::GetInstance()->lastY - static_cast<float>(ypos);
			}
			GlfwWindow::GetInstance()->lastX = static_cast<float>(xpos);
			GlfwWindow::GetInstance()->lastY = static_cast<float>(ypos);
			GlfwWindow::GetInstance()->firstFlag = false;
			GlfwWindow::GetInstance()->MainCamera->ProcessMouseMovement(xoffset, yoffset);
		}
		else {
			GlfwWindow::GetInstance()->firstFlag = true;
		}
	}

	//void GlfwWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	//}

	//void GlfwWindow::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){

	//}
}
