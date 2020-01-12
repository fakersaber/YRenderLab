#ifndef _YRENDER_OPENGLRHI_GLFWWINDOW_H
#define _YRENDER_OPENGLRHI_GLFWWINDOW_H

#include <Public/Basic/YHeapObject.h>
#include <Public/OpenGLRHI/GLFW/glfw3.h>
#include <Public/OpenGLRHI/GLVAO.h>
#include <Public/OpenGLRHI/GLTexture.h>
#include <map>


class TriMesh;
class Image;
class Camera;
class ForwardRaster;

class GlfwWindow : public YHeapObject
{
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

public:
	enum class VAOTYPE : uint8_t {
		Sphere,
		Plane,
		Screen
	};

	GlfwWindow();
	GlfwWindow(const GlfwWindow& rhs) = delete;
	GlfwWindow& operator=(const GlfwWindow& rhs) = delete;
	float GetDeltaTime() const { return deltaTime; };
	bool Initial(const int width, const int height);
	void Run();
	VAO GetVAO(std::shared_ptr<TriMesh> mesh);
	VAO GetVAO(GlfwWindow::VAOTYPE CurType);
	GLTexture GetTexture(std::shared_ptr<Image> img);
	std::shared_ptr<Camera> GetCamera() const;
	void UpdateViewPort(unsigned int width, unsigned int height);
	
private:
	virtual ~GlfwWindow() = default;
	void ProcessInput(GLFWwindow* window);

	//static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	//static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);


private:
	unsigned int width;
	unsigned int height;
	GLFWwindow* window;
	float deltaTime;
	float lastX;
	float lastY;
	bool firstFlag; // check if you hold down mouse right mouse button for first time

	//Camera准备移除，从Pipline中获取
	std::shared_ptr<ForwardRaster> ForwardPipline;
	std::shared_ptr<Camera> MainCamera;
	std::map<std::weak_ptr<TriMesh>, VAO, std::owner_less<std::weak_ptr<TriMesh>>> mesh2VAO;
	std::map<std::weak_ptr<Image>, GLTexture, std::owner_less<std::weak_ptr<Image>>> img2tex;

public:
	auto GetViewPortW() ->decltype(width);
	auto GetViewPortH() ->decltype(height);
};




#endif