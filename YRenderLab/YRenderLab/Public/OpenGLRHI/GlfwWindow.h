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

	public:
		float GetDeltaTime() const { return deltaTime; };
		//float GetLastX() const { return lastX; }
		//float GetLastY() const { return lastY; }

	private:
		GlfwWindow();
		virtual ~GlfwWindow();

	private:
		static void ProcessInput(GLFWwindow* window);
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	private:
		GLFWwindow* window;
		float deltaTime;

		float lastX;
		float lastY;
		bool firstFlag; // check if you hold down mouse right mouse button for first time
	};
}



#endif