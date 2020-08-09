#ifndef _YRENDER_WINDOW_GLFW_H_
#define _YRENDER_WINDOW_GLFW_H_

#include <Public/Window/WindowInterface.h>

struct GLFWwindow;
class RHI;

class Window_GLFW : public IWindowInterface {
public:

	Window_GLFW();

	virtual ~Window_GLFW();

	virtual void Init(const unsigned int width, const unsigned int height, const std::shared_ptr<RHI>& RHIResource) final override;
	virtual void Shutdown() final override;
	virtual void Tick() final override;

private:
	unsigned int width;
	unsigned int height;
	GLFWwindow* window;
};



#endif