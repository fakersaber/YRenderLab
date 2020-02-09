#ifndef _YRENDER_VIEWER_SHADOWGEN_H_
#define _YRENDER_VIEWER_SHADOWGEN_H_

#include <unordered_map>

#include <Public/Basic/YHeapObject.h>
#include <Public/OpenGLRHI/GLShader.h>
#include <Public/OpenGLRHI/GLTexture.h>
#include <Public/OpenGLRHI/GLFBO.h>
#include <Public/Basic/weakPtr_hash.h>

class GlfwWindow;
class Scene;
class LightComponent;
class YObject;
class TriMesh;

class ShadowGen : public YHeapObject {
public:
	ShadowGen(std::shared_ptr<GlfwWindow> pGLWindow);
	void init();
	
	void Visit(std::shared_ptr<Scene> scene);

protected:
	virtual ~ShadowGen() = default;

private:
	void GenDirectionalDepthMap(const std::shared_ptr<Scene>& Scene,const GLFBO& DepthFBO, const std::shared_ptr<LightComponent>& lightComponent);
	void RenderDirectionalShadowMap(std::shared_ptr<YObject> root);



private:
	const int DepthMapSize;
	GLShader DirectionalLightShadow;
	GLShader PointLightShadow;
	GLShader SpotLightShadow;
	std::shared_ptr<GlfwWindow> pGLWindow;
	std::unordered_map<std::weak_ptr<LightComponent>, GLFBO, WeakHasher<LightComponent>,WeakEqualTo<LightComponent>> lightDepthFBOMap;

};

#endif