#ifndef _YRENDER_GEOMETRYGENERATOR_H
#define _YRENDER_GEOMETRYGENERATOR_H

#include <vector>

#include "Vertex.h"



namespace YRender {
	static class GeometryGenerator
	{
	public:
		struct MeshData
		{
			std::vector<Vertex>	vertices;
			std::vector<uint32_t>	indices;
		};

	public:
		static void CreateBox(float width, float height, float depth, MeshData &mesh);
	};
}



#endif
