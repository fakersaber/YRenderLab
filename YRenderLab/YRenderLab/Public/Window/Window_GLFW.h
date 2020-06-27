#ifndef _YRENDER_WINDOW_GLFW_H_
#define _YRENDER_WINDOW_GLFW_H_

#include <Public/Window/WindowInterface.h>

struct GLFWwindow;

class Window_GLFW : public IWindowInterface {
public:

	Window_GLFW();

	virtual ~Window_GLFW();

	virtual bool Initial(const int width, const int height) final override;
	virtual void Shutdown() final override;
	virtual void Run() final override;

private:
	unsigned int width;
	unsigned int height;
	GLFWwindow* window;
};



#endif