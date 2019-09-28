#ifndef _YRENDER_GEOMETRYGENERATOR_H
#define _YRENDER_GEOMETRYGENERATOR_H

#include <vector>
#include <Public/Basic/Vertex/Vertex.h>



namespace YRender {
	class GeometryGenerator
	{
	public:
		struct _MeshData
		{
			std::vector<Vertex>	vertices;
			std::vector<uint32_t> indices;
		};

	public:
		static void CreateBox(_MeshData &mesh);
	};

	using MeshData = GeometryGenerator::_MeshData;
}



#endif
