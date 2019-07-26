#ifndef _GEOMETRYGENERATOR_H
#define _GEOMETRYGENERATOR_H

#include <vector>
#include "YRVertex.h"



namespace YRender {
	static class GeometryGenerator
	{
	public:
		struct MeshData
		{
			std::vector<YRiVertex>	vertices;
			std::vector<uint32_t>	indices;
		};

	public:
		static void CreateBox(float width, float height, float depth, MeshData &mesh);
	};
}



#endif
