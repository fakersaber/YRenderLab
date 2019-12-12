#ifndef _YRENDER_VIEWER_RASTER_H_
#define _YRENDER_VIEWER_RASTER_H_

#include <Public/Basic/YHeapObject.h>
#include <Public/OpenGLRHI/GLShader.h>

namespace YRender {
	class Scene;
	class Camera;
	class EnviromentGen;
	class GlfwWindow;

	class Raster : public YHeapObject {
	protected:
		Raster(std::shared_ptr<Scene> scene, std::shared_ptr<EnviromentGen> enviromentGen, std::shared_ptr<GlfwWindow> pGLWindow)
			: 
			scene(scene), 
			enviromentGen(enviromentGen),
			pGLWindow(pGLWindow)
		{

		};
		virtual ~Raster() = default;

	public:
		virtual void Draw() = 0;
		virtual void Initial();

	protected:
		void UpdateUBO();//更新UBO数据
		void MapUBOToShader(const GLShader& shader);
		void UpdateEnvironment();
		void UpdateUBO_DirectionalLights();
		void UpdateUBO_Environment();
	public:
		

	protected:
		std::shared_ptr<Scene> scene;
		std::shared_ptr<GlfwWindow> pGLWindow;
		std::shared_ptr<EnviromentGen> enviromentGen;
	private:
		unsigned int directionalLightsUBO;
		unsigned int environmentUBO;
	};
}

#endif