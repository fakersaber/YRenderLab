#ifndef _YRENDER_VIEWER_DEFERREDRASTER_H_
#define _YRENDER_VIEWER_DEFERREDRASTER_H_


#include <Public/Viewer/Raster.h>
#include <Public/OpenGLRHI/GLShader.h>
#include <Public/OpenGLRHI/GLFBO.h>


class YObject;
class BSDF_StandardPBR;
class TriMesh;

class DeferredRaster : public Raster {
public:
	DeferredRaster(std::shared_ptr<Scene> scene, std::shared_ptr<GlfwWindow> pGlwindow);
	void SetCurShader(const GLShader& shader) { curShader = shader; };


protected:
	virtual void Draw() override;
	virtual void Initial() override;
	virtual void Resize(unsigned int width, unsigned int height) override;
	virtual ~DeferredRaster() = default;

	/*----------------------------All Shader initial----------------------------------------------*/
	void InitShader_GBuffer();
	void InitShader_Skybox();
	void InitShader_AmbientLight();
	void InitShader_Lights();
	void InitShader_TAA();
	void InitShader_PostProcess();
	/*--------------------------------------------------------------------------------------------*/

	/*----------------------------All Pass--------------------------------------------------------*/
	void Pass_GBuffer();
	void Pass_SkyBox();
	void Pass_AmbientLight();
	void Pass_Lights();
	void Pass_TAA();
	void Pass_PostProcess();
	/*--------------------------------------------------------------------------------------------*/

private:
	void RenderScene(std::shared_ptr<YObject> obj);
	void SetMaterial(std::shared_ptr<BSDF_StandardPBR> material);
	void RenderMesh(std::shared_ptr<TriMesh> mesh, const YGM::Transform& model);
private:
	GLShader curShader;
	GLShader GBuffer_StandardPBRShader;
	GLShader Skybox_Shader;
	GLShader DirectLight_Shader;
	GLShader AmbientLight_Shader;
	GLShader postProcessShader;


	GLFBO gbufferFBO;
	GLFBO windowFBO;

};

#endif