#include "GeometryGenerator.h"


namespace YRender {
	void YRender::GeometryGenerator::CreateBox(float width, float height, float depth, _MeshData & mesh)
	{
		mesh.vertices.clear();
		mesh.indices.clear();

		mesh.indices.reserve(36);
		mesh.vertices.reserve(24);

		mesh.vertices.emplace_back();
	}
}
