#include <Public/SoftRHI/SoftRHI.h>


namespace YRender {


	SoftRender* SoftRender::GetRender() {
		static SoftRender instance;
		return &instance;
	}

	SoftRender::SoftRender() {
		_RenderDevice = RenderDevice::GetDevice();
	}

	SoftRender::~SoftRender() {

	}

	bool SoftRender::Initial(HWND hwnd, const int width, const int height) {
		if (!_RenderDevice->Initial(hwnd, width, height))
		{
			std::cout << "Create Device Failed" << std::endl;
			return false;
		}
		this->NdcToScreen(0, 0) = -width * 0.5f;
		this->NdcToScreen(0, 3) = width * 0.5f;
		this->NdcToScreen(1, 1) = -height * 0.5f;
		this->NdcToScreen(1, 3) = height * 0.5f;
		GeometryGenerator::CreateBox(this->Mesh);
		return true;
	}

	void SoftRender::Tick() {

	}

	void SoftRender::Render() {
		DrawIndexed(this->Mesh);
		_RenderDevice->DrawFrameBuffer();
	}


	bool SoftRender::BackFaceCulling(const Vertex& v1, const Vertex& v2, const Vertex& v3) {
		//以三角面指向观察者的方向为大拇指指向，其余手指逆时针绕着大拇指，如果手指的绕向和三角面顶点绕向一致，则这个面为正面，否则为背面。
		return true;
	}

	bool SoftRender::CVVClip(const Vector4& v) {
		if (v.x <= -v.w && v.x >= v.w &&
			v.y <= -v.w && v.y >= v.w &&
			v.z <= -v.w && v.z >= v.w)
		{
			return false;
		}
		return true;
	}

	void SoftRender::PerspecDivision(Vector4& ClipPos) {
		float z_reciprocal = 1.f / ClipPos.w;
		ClipPos.x *= z_reciprocal;
		ClipPos.y *= z_reciprocal;
		ClipPos.z *= z_reciprocal;
		ClipPos.w = 1.f;
	}



	Vertex SoftRender::VertexShader(const Vertex& vertex) {
		Vertex Out;
		Out.Position = MainCamera.GetProjectMatrix() * MainCamera.GetViewMatrix() * vertex.Position;
		return Out;
	}


	void SoftRender::NDCToScreen(Vector4& NdcVertex) {
		NdcVertex = NdcToScreen * NdcVertex;
	}


	//bresenham画线
	void SoftRender::BresenhamDrawLine(int x1, int y1, int x2, int y2)
	{
		int dx = x2 - x1;
		int dy = y2 - y1;
		int stepx = 1;
		int stepy = 1;
		if (dx >= 0)
		{
			stepx = 1;
		}
		else
		{
			stepx = -1;
			dx = abs(dx);
		}
		if (dy >= 0)
		{
			stepy = 1;
		}
		else
		{
			stepy = -1;
			dy = abs(dy);
		}

		int deltaX = 2 * dx;
		int deltaY = 2 * dy;
		if (dx > dy)
		{
			int p = deltaY - dx; //2Δy - Δx
			for (int i = 0; i <= dx; ++i)
			{
				_RenderDevice->DrawPixel(x1, y1);
				if (p >= 0)
				{
					p -= deltaX; //pi>0  pi+1 = pi + 2Δy - 2Δx
					y1 += stepy;
				}
				p += deltaY;
				x1 += stepx;
			}
		}
		else
		{
			int p = deltaX - dy;//2Δx - Δy
			for (int i = 0; i <= dy; i++)
			{
				_RenderDevice->DrawPixel(x1, y1);
				if (p >= 0)
				{
					p -= deltaY;
					x1 += stepx;
				}
				p += deltaX;
				y1 += stepy;
			}
		}
	}


	void SoftRender::DrawIndexed(const MeshData& mesh) {
		for (auto i = 0; i < mesh.indices.size(); i += 3) {
			//if (BackFaceCulling(mesh.vertices[mesh.indices[i]], mesh.vertices[mesh.indices[i + 1]], mesh.vertices[mesh.indices[i + 2]]))
			//	continue;
			//如果有一个顶点没在视锥体类，丢弃三角面
			if (MainCamera.GetCameraMode() == Camera::Project_Type::Perspective) {
				//裁剪坐标系下的坐标
				auto ClipVertex1 = VertexShader(mesh.vertices[mesh.indices[i]]);
				auto ClipVertex2 = VertexShader(mesh.vertices[mesh.indices[i + 1]]);
				auto ClipVertex3 = VertexShader(mesh.vertices[mesh.indices[i + 2]]);

				if (!CVVClip(ClipVertex1.Position) || !CVVClip(ClipVertex2.Position) || !CVVClip(ClipVertex3.Position)) {
					continue;
				}

				//透视除法
				PerspecDivision(ClipVertex1.Position);
				PerspecDivision(ClipVertex2.Position);
				PerspecDivision(ClipVertex3.Position);


				NDCToScreen(ClipVertex1.Position);
				NDCToScreen(ClipVertex2.Position);
				NDCToScreen(ClipVertex3.Position);

				BresenhamDrawLine(
					static_cast<int>(ClipVertex1.Position.x),
					static_cast<int>(ClipVertex1.Position.y),
					static_cast<int>(ClipVertex2.Position.x),
					static_cast<int>(ClipVertex2.Position.y)
				);
				BresenhamDrawLine(
					static_cast<int>(ClipVertex1.Position.x),
					static_cast<int>(ClipVertex1.Position.y),
					static_cast<int>(ClipVertex3.Position.x),
					static_cast<int>(ClipVertex3.Position.y)
				);
				BresenhamDrawLine(
					static_cast<int>(ClipVertex2.Position.x),
					static_cast<int>(ClipVertex2.Position.y),
					static_cast<int>(ClipVertex3.Position.x),
					static_cast<int>(ClipVertex3.Position.y)
				);
			}
		}
	}
}