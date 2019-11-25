#ifndef _YRENDER_VIEWER_ENVIROMENTGEN_H_
#define _YRENDER_VIEWER_ENVIROMENTGEN_H_

#include <Public/Basic/YHeapObject.h>
#include <Public/OpenGLRHI/GLShader.h>


namespace YRender {
	class Scene;


	class EnviromentGen : public YHeapObject {
	public:
		EnviromentGen(){}

	protected:
		virtual ~EnviromentGen() = default;

	public:
		void Init();
		void Visit(std::shared_ptr<Scene> scene);

	private:
		void InitShaders();
		void InitShader_Skybox();


	private:
		GLShader shader_genSkybox;
	};
}



#endif