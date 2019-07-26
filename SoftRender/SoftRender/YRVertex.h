#pragma once


namespace YRender {
	class YRiVertex
	{
	public:
		YRiVertex();
		~YRiVertex();

		YRiVertex(
			float px, float py, float pz, //position
			float nx, float ny, float nz, //normal
			float tx, float ty, float tz, //tangent
			float u, float v)             //texture
		{
			Position[0] = px;
			Position[1] = py;
			Position[2] = pz;
			Normal[0] = nx;
			Normal[1] = ny;
			Normal[2] = nz;
			TangentU[0] = tx;
			TangentU[1] = ty;
			TangentU[2] = tz;
			UV[0] = u;
			UV[1] = v;
		}

		float Position[3];
		float UV[2];
		float Normal[3];
		float TangentU[3];
	};
}
