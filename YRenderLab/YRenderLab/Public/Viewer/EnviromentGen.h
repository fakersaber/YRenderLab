#ifndef _YRENDER_VIEWER_ENVIROMENTGEN_H_
#define _YRENDER_VIEWER_ENVIROMENTGEN_H_

#include <Public/Basic/YHeapObject.h>
#include <Public/OpenGLRHI/GLShader.h>
#include <Public/OpenGLRHI/GLTexture.h>

namespace YRender {
	class Scene;
	class GLTexture;

	class EnviromentGen : public YHeapObject {
	public:
		EnviromentGen(){}

	protected:
		virtual ~EnviromentGen() = default;

	public:
		void Init();
		void Visit(std::shared_ptr<Scene> scene);
		std::shared_ptr <GLTexture> GetSkyBox() const { return SkyBox; }

	private:
		//IBL sky box
		void InitShaders();
		void InitGenShader_IBLSkybox();
		void InitGenShader_Irradiance();
		void InitGenShader_PreFilter();

		//ordinary sky box 
		void InitSkyBoxTexture();
		

	private:
		GLShader shader_genIBLSkybox;
		std::shared_ptr<GLTexture> SkyBox;
	};
}



#endif