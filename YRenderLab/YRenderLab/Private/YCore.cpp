#include <Public/YCore.h>
#include <Public/Basic/BasicCore.h>
#include <Public/OpenGLRHI/GlfwWindow.h>


//#define SOFT_RENDER
#define OPENGL_RENDER

YCore* YCore::GetCore() {
	static YCore instance;
	return &instance;
}


YCore::YCore() {
#ifdef SOFT_RENDER
	this->_RenderWindow = Win32Window::GetInstance();
#elif defined(OPENGL_RENDER)
	this->pGLInstance = New<GlfwWindow>();
#endif 
}

YCore::~YCore() {

}


bool YCore::Initial(const int width, const int height) {

	if (!pGLInstance->Initial(width, height)){
		std::cout << "Create Window Failed" << std::endl;
		return false;
	}
	return true;
}

void YCore::Run() {

	//RayCamera::TestRayTracer();
	pGLInstance->Run();
}

std::shared_ptr<GlfwWindow> YCore::GetGLWindow() const
{
	return pGLInstance;
}






//static void ProcessInput(GLFWwindow* window) {
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, true);
//	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//		YCore::GetCore()->GetGLWindow()->GetCamera()->ProcessKeyboard(Camera::ENUM_Movement::MOVE_FORWARD, GlfwWindow::GetInstance()->GetDeltaTime());
//	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//		YCore::GetCore()->GetGLWindow()->GetCamera()->ProcessKeyboard(Camera::ENUM_Movement::MOVE_BACKWARD, GlfwWindow::GetInstance()->GetDeltaTime());
//	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//		YCore::GetCore()->GetGLWindow()->GetCamera()->ProcessKeyboard(Camera::ENUM_Movement::MOVE_LEFT, GlfwWindow::GetInstance()->GetDeltaTime());
//	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//		YCore::GetCore()->GetGLWindow()->GetCamera()->ProcessKeyboard(Camera::ENUM_Movement::MOVE_RIGHT, GlfwWindow::GetInstance()->GetDeltaTime());
//}


