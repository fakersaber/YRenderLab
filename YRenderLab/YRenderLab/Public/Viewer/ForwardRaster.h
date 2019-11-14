#ifndef _YRENDER_VIEWER_FORWARDRASTER_H_
#define _YRENDER_VIEWER_FORWARDRASTER_H_

#include <Public/Viewer/Raster.h>
#include <Public/YGM/Transform.h>

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
	};
}

#endif