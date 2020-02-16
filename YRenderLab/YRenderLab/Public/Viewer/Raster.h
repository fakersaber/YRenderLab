#ifndef _YRENDER_VIEWER_RASTER_H_
#define _YRENDER_VIEWER_RASTER_H_

#include <Public/Basic/YHeapObject.h>
#include <Public/OpenGLRHI/GLShader.h>


class Scene;
class Camera;
class EnviromentGen;
class ShadowGen;
class GlfwWindow;


class TriMesh;
class Cube;
class Plane;

class BSDF_StandardPBR;
class BSDF_Emission;

class Raster : public YHeapObject {
protected:
	Raster(std::shared_ptr<Scene> scene, 
		std::shared_ptr<EnviromentGen> enviromentGen, 
		std::shared_ptr<ShadowGen> shadowGen,
		std::shared_ptr<GlfwWindow> pGLWindow
	)
		:
		scene(scene),
		enviromentGen(enviromentGen),
		shadowGen(shadowGen),
		pGLWindow(pGLWindow)
	{

	};
	virtual ~Raster() = default;

public:
	virtual void Draw() = 0;
	virtual void Initial();
	virtual void Resize(unsigned int width, unsigned int height) = 0;
	virtual void RenderMesh(std::shared_ptr<TriMesh> Primitive, const YGM::Transform& model) = 0;
	virtual void RenderMesh(std::shared_ptr<Cube> Primitive, const YGM::Transform& model) = 0;
	virtual void RenderMesh(std::shared_ptr<Plane> Primitive, const YGM::Transform& model) = 0;
	virtual void SetMaterial(std::shared_ptr<BSDF_StandardPBR> StandardMateril) = 0;
	virtual void SetMaterial(std::shared_ptr<BSDF_Emission> EmissionMaterial) = 0;
protected:
	void MapUBOToShader(const GLShader& shader);
	void UpdateUBO_Camera();
	void UpdateUBO_DirectionalLights();
	void UpdateUBO_Environment();
public:


protected:
	std::shared_ptr<Scene> scene;
	std::shared_ptr<GlfwWindow> pGLWindow;
	std::shared_ptr<EnviromentGen> enviromentGen;
	std::shared_ptr<ShadowGen> shadowGen;
	std::vector<YGM::Transform> ObjectTransformVec;
private:
	unsigned int directionalLightsUBO;
	unsigned int environmentUBO;
	unsigned int cameraUBO;


	//unsigned int StandardMaterialPassUBO;
	//unsigned int EnviromentPassUBO;
	//unsigned int TaaPassUBO;
	//unsigned int LightPassUBO;
};


#endif