#include <Public/SoftRHI/SoftRHI.h>
#include <algorithm>

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
		this->width = width;
		this->height = height;
		this->NdcToScreen(0, 0) = (width - 1) * 0.5f;
		this->NdcToScreen(0, 3) = (width - 1)  * 0.5f;
		this->NdcToScreen(1, 1) = (height - 1) * 0.5f;
		this->NdcToScreen(1, 3) = (height - 1) * 0.5f;
		GeometryGenerator::CreateBox(this->Mesh);
		return true;
	}

	void SoftRender::Tick() {

	}

	void SoftRender::Render() {
		DrawIndexed(this->Mesh);
		_RenderDevice->DrawFrameBuffer();
	}


	bool SoftRender::BackFaceCulling(const Vector4& v1, const Vector4& v2, const Vector4& v3) {
		//逆时针方向为正，叉积值大于0，右手大拇指方向指向人
		//auto preCross = [](const Vector2i& p0, const Vector2i& p1, const Vector2i& CurPoint) ->int {
		//	//组成向量a = p1 - p0，b = cur - p0,  然后右手从a到b的叉积，p0均为原点
		//	return (p1.x - p0.x)*(CurPoint.y - p0.y) - (CurPoint.x - p0.x)*(p1.y - p0.y);
		//};

		//与preCross完全相同，只是为了求叉积的正负
		//返回false代表不剔除，true代表剔除
		Vector2 EdgVec0(v2.x - v1.x, v2.y - v1.y);
		Vector2 EdgVec1(v3.x - v1.x, v3.y - v1.y);
		if (EdgVec0.x * EdgVec1.y - EdgVec0.y * EdgVec1.x > 0.f)
			return false;
		return true;
	}

	bool SoftRender::CVVClip(const Vector4& v) {
		if (v.x >= -v.w && v.x <= v.w &&
			v.y >= -v.w && v.y <= v.w &&
			v.z >= -v.w && v.z <= v.w
			)
		{
			return true;
		}
		return false;
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
				if (x1 >= 0 && x1 < this->width && y1 >= 0 && y1 < this->height)
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
				if (x1 >= 0 && x1 < this->width && y1 >= 0 && y1 < this->height)
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

			//如果有一个顶点没在视锥体内，丢弃三角面
			if (MainCamera.GetCameraMode() == Camera::Project_Type::Perspective) {
				//背面剔除放在这里，完全根据模型给定的顶点数据裁剪，不管视角如何变换都没有用,所以不正确。
				//if (BackFaceCulling(mesh.vertices[mesh.indices[i]].Position, mesh.vertices[mesh.indices[i + 1]].Position, mesh.vertices[mesh.indices[i + 2]].Position))
				//	continue;

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

				if (BackFaceCulling(ClipVertex1.Position, ClipVertex2.Position, ClipVertex3.Position))
					continue;

				HalfSpaceTriangle(ClipVertex1, ClipVertex2, ClipVertex3);

				//BresenhamDrawLine(
				//	ClipVertex1.Position.x,
				//	ClipVertex1.Position.y,
				//	ClipVertex2.Position.x,
				//	ClipVertex2.Position.y
				//);

				//BresenhamDrawLine(
				//	ClipVertex1.Position.x,
				//	ClipVertex1.Position.y,
				//	ClipVertex3.Position.x,
				//	ClipVertex3.Position.y
				//);
				//BresenhamDrawLine(
				//	ClipVertex2.Position.x,
				//	ClipVertex2.Position.y,
				//	ClipVertex3.Position.x,
				//	ClipVertex3.Position.y
				//);
			}
		}
	}

	void SoftRender::HalfSpaceTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2) {
		//叉积值大于0，右手大拇指方向指向人
		auto preCross = [](const Vector2i& p0, const Vector2i& p1, const Vector2i& p2) ->int {
			return (p1.x - p0.x)*(p2.y - p0.y) - (p2.x - p0.x)*(p1.y - p0.y);
		};



		//计算三角形的包围盒
		Vector2i MinXY, MaxXY;
		Vector2i ScreenPos0(v0.Position.x, v0.Position.y);
		Vector2i ScreenPos1(v1.Position.x, v1.Position.y);
		Vector2i ScreenPos2(v2.Position.x, v2.Position.y);

		MinXY.x = YGM::Math::Max(YGM::Math::Min(YGM::Math::Min(ScreenPos0.x, ScreenPos1.x), ScreenPos2.x), 0);
		MinXY.y = YGM::Math::Max(YGM::Math::Min(YGM::Math::Min(ScreenPos0.y, ScreenPos1.y), ScreenPos2.y), 0);
		MaxXY.x = YGM::Math::Min(YGM::Math::Max(YGM::Math::Max(ScreenPos0.x, ScreenPos1.x), ScreenPos2.x), this->width - 1);
		MaxXY.y = YGM::Math::Min(YGM::Math::Max(YGM::Math::Max(ScreenPos0.y, ScreenPos1.y), ScreenPos2.y), this->height - 1);

		for (int i = MinXY.x; i < MaxXY.x; ++i) {
			for (int j = MinXY.y; j < MaxXY.y; ++j) {
				Vector2i CurPoint(i, j);
				auto e1 = preCross(ScreenPos0, ScreenPos1, CurPoint);
				auto e2 = preCross(ScreenPos1, ScreenPos2, CurPoint);
				auto e3 = preCross(ScreenPos2, ScreenPos0, CurPoint);

				if (e1 > 0 && e2 > 0 && e3 > 0) {
					_RenderDevice->DrawPixel(i, j);
				}
			}
		}
	}
}