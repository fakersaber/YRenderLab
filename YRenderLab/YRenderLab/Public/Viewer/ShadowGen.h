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
class Plane;

class ShadowGen : public YHeapObject {
public:
	ShadowGen(std::shared_ptr<GlfwWindow> pGLWindow);
	void Init();
	void UpdateShadowMap(std::shared_ptr<Scene> scene);
	const YGM::Transform& GetDirectionalLightProjView(const std::shared_ptr<LightComponent>& DirectionalLight) const;
	GLTexture GetDirectionalLightShadowMap(const std::shared_ptr<LightComponent>& DirectionalLight) const;

	void RenderMesh(std::shared_ptr<TriMesh> mesh, const YGM::Transform& model);
	void RenderMesh(std::shared_ptr<Plane> plane, const YGM::Transform& model);

protected:
	virtual ~ShadowGen() = default;

private:
	void GenDirectionalDepthMap(const std::shared_ptr<Scene>& Scene,const GLFBO& DepthFBO, const std::shared_ptr<LightComponent>& lightComponent);
	void RenderDirectionalShadowMap(std::shared_ptr<YObject> root);
	void SetCurShader(const GLShader& shader);

private:
	const int DepthMapSize;

	GLShader CurShader;
	GLShader DirectionalLightShadow_Shader;
	GLShader PointLightShadow_Shader;
	GLShader SpotLightShadow_Shader;
	std::shared_ptr<GlfwWindow> pGLWindow;
	std::unordered_map<std::weak_ptr<LightComponent>, GLFBO, WeakHasher<LightComponent>,WeakEqualTo<LightComponent>> DirectionalDepthFBOMap;
	std::unordered_map<std::weak_ptr<LightComponent>, YGM::Transform, WeakHasher<LightComponent>, WeakEqualTo<LightComponent>> DirectionalLightProjView;
};

#endif