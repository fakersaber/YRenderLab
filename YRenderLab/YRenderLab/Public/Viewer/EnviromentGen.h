#ifndef _YRENDER_VIEWER_ENVIROMENTGEN_H_
#define _YRENDER_VIEWER_ENVIROMENTGEN_H_

#include <Public/Basic/YHeapObject.h>
#include <Public/OpenGLRHI/GLShader.h>
#include <Public/OpenGLRHI/GLTexture.h>
#include <Public/OpenGLRHI/GLFBO.h>


namespace YRender {
	class Scene;
	class GlfwWindow;

	class EnviromentGen : public YHeapObject {
	public:
		EnviromentGen(std::shared_ptr<GlfwWindow> pGLWindow);

	protected:
		virtual ~EnviromentGen() = default;

	public:
		void Init();
		void Visit(std::shared_ptr<Scene> scene);
		std::shared_ptr <GLTexture> GetSkyBox() const { return OldSkyBox; }

	private:
		//IBL sky box
		void InitShaders();
		void InitFBOs();
		void InitGenShader_IBLSkybox();
		void InitGenShader_Irradiance();
		void InitGenShader_PreFilter();
		void UpdateSkyBox();
		void UpdateIrradianceMap();
		
		//ordinary sky box 
		void InitSkyBoxTexture();
		

	private:
		std::shared_ptr<GLTexture> OldSkyBox;
		int skyboxSize;
		int irradianceSize;
		int prefilterSize;
		int brdfSize;

	public:
		GLShader shader_genIBLSkybox;
		GLShader shader_genIrradianceMap;
		GLShader shader_genPrefilterMap;
		GLFBO genSkyboxFBO;
		GLFBO genIrradianceFBO;
		GLTexture skyBox;
		GLTexture irradianceMap;
		//std::weak_ptr<Image> curImg;

		std::shared_ptr<Image> curImg;

		std::shared_ptr<GlfwWindow> pGLWindow;
		static const YGM::Transform captureViews[6];
		static const GLFBO::TexRenderTarget mapper[6];
	};
}



#endif