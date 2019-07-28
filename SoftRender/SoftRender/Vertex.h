#ifndef _YRENDER_VERTEX_H
#define _YRENDER_VERTEX_H

#include <YGM/Vector2.hpp>
#include <YGM/Vector3.hpp>
#include <YGM/Vector4.hpp>


namespace YRender {
	class Vertex
	{
	public:
		Vertex();
		~Vertex();

		Vertex(Vector4& position, Vector3& normal)
			: Position(position), Normal(normal)
		{}


		Vertex(Vector4& position, Vector3& normal, Vector3& tangent, Vector3& Color, Vector2& uv)
			:Position(position), Normal(normal), Tangent(tangent), Color(Color), UV(uv)
		{}

		Vector4 Position;
		Vector3 Normal;
		Vector3 Tangent;
		Vector3 Color;
		Vector2 UV;
	};
}

#endif