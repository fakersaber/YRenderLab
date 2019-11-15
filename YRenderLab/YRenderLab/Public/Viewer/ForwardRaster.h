#ifndef _YRENDER_VIEWER_FORWARDRASTER_H_
#define _YRENDER_VIEWER_FORWARDRASTER_H_

#include <Public/Viewer/Raster.h>
#include <Public/YGM/Transform.h>

#include <Public/OpenGLRHI/GLVAO.h>

#include <vector>
#include <unordered_map>
namespace YRender {
	class YObject;
	
	class TriMesh;

	class BSDF_Diffuse;

	class ForwardRaster : public Raster {
	public:
		ForwardRaster();

	public:
		virtual void Draw() override;

	protected:
		virtual ~ForwardRaster() = default;

	protected:
		virtual void Visit(std::shared_ptr<YObject> obj);

		//set shader for target
		virtual void Visit(std::shared_ptr<BSDF_Diffuse> bsdf);

		void Visit(std::shared_ptr<TriMesh> mesh);

	private:
		std::vector<YGM::Transform> modelVec;

		//这样创建表一直变大，且如果TriMesh被释放后，对于容器也是不可知的，如何要这样缓存，需要定时清理。
		std::unordered_map<std::weak_ptr<TriMesh>, VAO> mesh2VAO;
		GLShader curShader;
	};
}

#endif