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
		GLTexture GetSkyBox() const { return SkyBox; }

	private:
		//IBL sky box
		void InitShaders();
		void InitGenShader_IBLSkybox();


		//ordinary sky box 
		void InitSkyBoxTexture();
		

	private:
		GLShader shader_genIBLSkybox;
		GLTexture SkyBox;
	};
}



#endif