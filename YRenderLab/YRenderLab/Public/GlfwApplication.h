#pragma once

#include <Public/YApplication.h>

struct GLFWwindow;
class RHI;
class IPipeline;

class GlfwApplication final : public YApplication {
public:
	GlfwApplication() = default;
	virtual ~GlfwApplication() = default;
	virtual void Init(const int width, const int height) final override;
	virtual void Shutdown() final override;
	virtual void Update() final override;
	virtual void Render() final override;
	virtual void Tick() final override;

private:
	unsigned int width;
	unsigned int height;
	RHI* RenderRHI;
	IPipeline* RenderPipeline;
	GLFWwindow* window;
};