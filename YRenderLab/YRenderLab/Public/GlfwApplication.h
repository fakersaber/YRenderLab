#pragma once

#include <Public/YApplication.h>

struct GLFWwindow;

class GlfwApplication final : public YApplication {
public:
	GlfwApplication() = default;
	virtual ~GlfwApplication() = default;
	virtual void Init(const int width, const int height) final override;
	virtual void Shutdown() final override;
	virtual void Tick() final override;

protected:
	void Update();
	void Render();
private:
	unsigned int width;
	unsigned int height;

	//[Resource management]
	GLFWwindow* window;
};