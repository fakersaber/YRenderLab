#ifndef _YRENDER_VIEWER_SHADOWGEN_H_
#define _YRENDER_VIEWER_SHADOWGEN_H_

#include <Public/Basic/YHeapObject.h>
#include <Public/OpenGLRHI/GLShader.h>
#include <Public/OpenGLRHI/GLTexture.h>
#include <Public/OpenGLRHI/GLFBO.h>

class GlfwWindow;
class Scene;

class ShadowGen : public YHeapObject {
public:
	ShadowGen(std::shared_ptr<GlfwWindow> pGLWindow);
	void init();
	void Visit(std::shared_ptr<Scene> scene);

protected:
	virtual ~ShadowGen() = default;


private:
	const int DepthMapSize;
	GLShader DirectionalLightShadow;
	GLShader PointLightShadow;
	GLShader SpotLightShadow;
	std::shared_ptr<GlfwWindow> pGLWindow;
};

#endif