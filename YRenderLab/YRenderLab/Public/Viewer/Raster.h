#ifndef _YRENDER_VIEWER_RASTER_H_
#define _YRENDER_VIEWER_RASTER_H_

#include <Public/Basic/YHeapObject.h>
#include <Public/OpenGLRHI/GLShader.h>
#include <Public/Viewer/EnviromentGen.h>

namespace YRender {
	class Scene;
	class Camera;

	class Raster : public YHeapObject {
	protected:
		Raster(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera,std::shared_ptr<EnviromentGen> enviromentGen)
			: scene(scene), 
			camera(camera),
			enviromentGen(enviromentGen)
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
		std::shared_ptr<Camera> camera;
		std::shared_ptr<EnviromentGen> enviromentGen;
	private:
		unsigned int directionalLightsUBO;
		unsigned int environmentUBO;
	};
}

#endif