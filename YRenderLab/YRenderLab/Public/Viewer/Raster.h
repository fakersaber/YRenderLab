#ifndef _YRENDER_VIEWER_RASTER_H_
#define _YRENDER_VIEWER_RASTER_H_

#include <Public/Basic/YHeapObject.h>
namespace YRender {
	class Scene;
	class GLShader;
	class Raster : public YHeapObject {
	protected:
		Raster(std::shared_ptr<Scene> scene) : scene(scene) {};
		virtual ~Raster() = default;

	public:
		virtual void Draw() = 0;
		virtual void Initial();

	protected:
		void UpdateUBO();//更新各种UBO
		void MapUBOToShader(const GLShader& shader);
		void UpdateUBO_DirectionalLights();

	protected:
		std::shared_ptr<Scene> scene;

	private:
		unsigned int directionalLightsUBO;
		unsigned int environmentUBO;
	};
}

#endif