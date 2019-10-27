#ifndef _YRENDER_OPENGLRHI_GLFWWINDOW_H
#define _YRENDER_OPENGLRHI_GLFWWINDOW_H


#include <Public/RenderWindow.h>
#include <Public/OpenGLRHI/GLFW/glfw3.h>
#include <Public/OpenGLRHI/GlRenderClass.h>


namespace YRender {
	class GlfwWindow : public RenderWindow
	{
	public:
		GlfwWindow(const GlfwWindow& rhs) = delete;
		GlfwWindow& operator=(const GlfwWindow& rhs) = delete;

	public:
		virtual bool Initial(const int width, const int height) override;
		virtual void Run() override;
		static GlfwWindow* GetInstance();

	private:
		GlfwWindow();
		virtual ~GlfwWindow();

	private:
		static void ProcessInput(GLFWwindow* window);
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	private:
		GLFWwindow* window;
	};
}



#endif