#ifndef _YRENDER_VIEWER_ENVIROMENTGEN_H_
#define _YRENDER_VIEWER_ENVIROMENTGEN_H_

#include <Public/Basic/YHeapObject.h>
#include <Public/OpenGLRHI/GLShader.h>
#include <Public/OpenGLRHI/GLTexture.h>
#include <Public/OpenGLRHI/GLFBO.h>


class Scene;
class GlfwWindow;

class EnviromentGen : public YHeapObject {
public:
	EnviromentGen(std::shared_ptr<GlfwWindow> pGLWindow);

protected:
	virtual ~EnviromentGen() = default;

public:
	void Init();
	void UpdateEnvironment(std::shared_ptr<Scene> scene);
	GLTexture GetSkyBox() const { return skyBox; }
	GLTexture GetIrradianceMap() const { return irradianceMap; }
	GLTexture GetPrefilterMap() const { return prefilterMap; }
	GLTexture GetBRDF_LUT() const { return genBrdfLutFBO.GetColorTexture(0); }


private:
	//IBL sky box
	void InitShaders();
	void InitFBOs();
	void InitGenShader_IBLSkybox();
	void InitGenShader_Irradiance();
	void InitGenShader_PreFilter();
	void InitGenShader_brdfLUT();
	void UpdateSkyBox();
	void UpdateIrradianceMap();
	void UpdatePreFilterMap();
	void UpdateBRDFLut();
	void Clear();



private:
	int skyboxSize;
	int irradianceSize;
	int prefilterSize;
	int brdfSize;

	bool isInitBRDFFBO;
public:
	static constexpr int maxMipLevels = 5;

	GLShader shader_genIBLSkybox;
	GLShader shader_genIrradianceMap;
	GLShader shader_genPrefilterMap;
	GLShader shader_genBRDFLUT;

	GLFBO genSkyboxFBO;
	GLFBO genIrradianceFBO;
	GLFBO prefilterFBOs[maxMipLevels];
	GLFBO genBrdfLutFBO;

	//存在FBO的texture下后应该可以不用再存到这里
	GLTexture skyBox;
	GLTexture irradianceMap;
	GLTexture prefilterMap;
	std::weak_ptr<Image> curImg;



	std::shared_ptr<GlfwWindow> pGLWindow;
	static const YGM::Transform captureViews[6];
	static const GLFBO::RenderTargetType mapper[6];
};



#endif