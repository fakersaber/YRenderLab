#ifndef _YRENDER_VERTEX_H
#define _YRENDER_VERTEX_H

#include <YGM/Vector3.hpp>

namespace YRender {
	class Vertex
	{
	public:
		Vertex();
		~Vertex();

		Vertex(Vector3& position,Vector3& normal,Vector3& tangent,Vector3& uv)
			:Position(position), Normal(normal), Tangent(tangent), UV(uv)
		{}

		Vector3 Position;
		Vector3 Normal;
		Vector3 Tangent;
		Vector3 UV;
	};
}

#endif