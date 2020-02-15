#ifndef _YRENDER_OPENGLRHI_GLFWWINDOW_H
#define _YRENDER_OPENGLRHI_GLFWWINDOW_H

#include <Public/YCore.h>
#include <Public/Basic/YHeapObject.h>
#include <Public/OpenGLRHI/GLFW/glfw3.h>
#include <Public/OpenGLRHI/GLVAO.h>
#include <Public/OpenGLRHI/GLTexture.h>
#include <unordered_map>
#include <map>


class TriMesh;
class Image;
class Camera;
class Raster;
//class ForwardRaster;
//class DeferredRaster;

class GlfwWindow : public YHeapObject
{
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

public:
	void Run();
	GlfwWindow();
	GlfwWindow(const GlfwWindow& rhs) = delete;
	GlfwWindow& operator=(const GlfwWindow& rhs) = delete;
	float GetDeltaTime() const { return deltaTime; };
	bool Initial(const int width, const int height);
	VAO GetVAO(std::shared_ptr<TriMesh> mesh);
	VAO GetVAO(CoreDefine::StaticVAOType CurType);
	GLTexture GetTexture(std::shared_ptr<Image> img);
	std::shared_ptr<Camera> GetCamera() const;
	void UpdateViewPort(unsigned int width, unsigned int height);
	std::shared_ptr<Raster> GetRaster() const;
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
	std::shared_ptr<Raster> RenderRaster;
	//#TODO：Camera准备移除，从Pipline中获取
	std::shared_ptr<Camera> MainCamera;
	std::map<std::weak_ptr<TriMesh>, VAO, std::owner_less<std::weak_ptr<TriMesh>>> mesh2VAO;
	std::map<std::weak_ptr<Image>, GLTexture, std::owner_less<std::weak_ptr<Image>>> img2tex;

	std::unordered_map<CoreDefine::StaticVAOType, VAO> StaticMeshContainer;

public:
	auto GetViewPortW() ->decltype(width);
	auto GetViewPortH() ->decltype(height);
};




#endif