#ifndef _YRENDER_BASIC_MESH_PLANE_H_
#define _YRENDER_BASIC_MESH_PLANE_H_

#include <Public/YGM/Vector2.hpp>
#include <Public/YGM/Vector3.hpp>
#include <Public/Basic/Mesh/Primitive.h>
#include <vector>

class Plane : public Primitive {

public:
	Plane();
	const std::vector<Vector3>& GetPositions() const { return position; }
	const std::vector<Vector3>& GetNormals() const { return normal; }
	const std::vector<Vector2>& GetTexcoords() const { return texcoords; }
	const std::vector<unsigned int>& GetIndice() const { return indice; }

	virtual void RenderPrimitive(const std::shared_ptr<Raster>& raster, const YGM::Transform& model) override;
	virtual void RenderShadowPrimitive(const std::shared_ptr<ShadowGen>& shadowgen, const YGM::Transform& model) override;

protected:
	virtual ~Plane() = default;

protected:
	std::vector<unsigned int> indice;
	std::vector<Vector3> position;
	std::vector<Vector3> normal;
	std::vector<Vector2> texcoords;

	static const float PlaneData[32];

};




#endif
