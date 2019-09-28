#ifndef _YRENDER_VERTEX_H
#define _YRENDER_VERTEX_H

#include <Public/YGM/Vector2.hpp>
#include <Public/YGM/Vector3.hpp>
#include <Public/YGM/Vector4.hpp>


namespace YRender {
	class Vertex
	{
	public:
		Vertex();
		~Vertex();

		Vertex(Vector4& position, Vector3& normal)
			: Position(position), Normal(normal), PosH(0.f)
		{}


		Vertex(Vector4& position, Vector3& normal, Vector3& tangent, Vector3& Color, Vector2& uv)
			:Position(position), Normal(normal), Tangent(tangent), Color(Color), UV(uv), PosH(0.f)
		{}

		Vector4 Position;
		Vector3 Normal;
		Vector3 Tangent;
		Vector3 Color;
		Vector2 UV;
		float PosH;
	};
}

#endif