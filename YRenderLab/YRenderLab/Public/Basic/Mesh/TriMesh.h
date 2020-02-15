#ifndef _YRENDER_BASIC_MESH_TRIMESH_H_
#define _YRENDER_BASIC_MESH_TRIMESH_H_

#include <Public/Basic/Mesh/Primitive.h>
#include <Public/YGM/Vector3.hpp>
#include <Public/YGM/Vector2.hpp>
#include <vector>


class TriMesh final : public Primitive {
public:
	TriMesh(
		const std::vector<unsigned int>& indice,
		const std::vector<Vector3>& position,
		const std::vector<Vector3>& normal,
		const std::vector<Vector2>& texcoords,
		const std::vector<Vector3>& tangents = std::vector<Vector3>()
	);

	TriMesh(
		std::vector<unsigned int>&& indice,
		std::vector<Vector3>&& position,
		std::vector<Vector3>&& normal,
		std::vector<Vector2>&& texcoords,
		std::vector<Vector3>&& tangents = std::vector<Vector3>()
	);

	TriMesh(
		const int indiceNum,
		const int vertexNum,
		const unsigned int* indice,
		const Vector3* position,
		const Vector3* normal,
		const Vector2* texcoords,
		const Vector3* tangents = nullptr
	);

protected:
	virtual void InitAfterNew() override;
	virtual ~TriMesh() = default;
public:
	virtual void RenderPrimitive(const std::shared_ptr<Raster>& raster, const YGM::Transform& model) override;

	virtual void RenderShadowPrimitive(const std::shared_ptr<ShadowGen>& shadowgen, const YGM::Transform& model) override;

	const std::vector<Vector3>& GetPositions() const { return position; }
	const std::vector<Vector3>& GetNormals() const { return normal; }
	const std::vector<Vector2>& GetTexcoords() const { return texcoords; }
	const std::vector<Vector3>& GetTangents() const { return tangents; }
	const std::vector<unsigned int>& GetIndice() const { return indice; }
private:
	std::vector<unsigned int> indice;
	std::vector<Vector3> position;
	std::vector<Vector3> normal;
	std::vector<Vector2> texcoords;
	std::vector<Vector3> tangents;

	//BoundBox °ó¶¨ºÐ´ýÐø

};




#endif