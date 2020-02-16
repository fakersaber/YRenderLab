#include <Public/Basic/Mesh/Plane.h>
#include <Public/Viewer/Raster.h>
#include <Public/Viewer/ShadowGen.h>

const float Plane::PlaneData[32] = {
	// positions          // normals           // texture coords
	-0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, //left front
	 0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, //right front
	-0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, //left back
	 0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, //right back
};


Plane::Plane(){
	indice.reserve(6);
	position.reserve(4);
	normal.reserve(4);
	texcoords.reserve(4);
	for (uint32_t i = 0; i < 4; ++i) {
		position.emplace_back(PlaneData[8 * i], PlaneData[8 * i + 1], PlaneData[8 * i + 2]);
		normal.emplace_back(PlaneData[8 * i + 3], PlaneData[8 * i + 4], PlaneData[8 * i + 5]);
		texcoords.emplace_back(PlaneData[8 * i + 6], PlaneData[8 * i + 7]);
	}

	//左下起始顶点
	const uint32_t squarIndex[6] = {
		0,1,2,
		3,2,1
	};
	for (uint32_t i = 0; i < 6; ++i) {
		indice.emplace_back(squarIndex[i]);
	}
}

void Plane::RenderPrimitive(const std::shared_ptr<Raster>& raster, const YGM::Transform & model){
	raster->RenderMesh(shared_this<Plane>(), model);
}

void Plane::RenderShadowPrimitive(const std::shared_ptr<ShadowGen>& shadowgen, const YGM::Transform & model){
	shadowgen->RenderMesh(shared_this<Plane>(), model);
}
