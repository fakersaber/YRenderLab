#include <Public/SoftRHI/SoftRHI.h>


//SoftRender* SoftRender::GetRender() {
//	static SoftRender instance;
//	return &instance;
//}

SoftRender::SoftRender(HWND hwnd) : hwnd(hwnd) { }

SoftRender::~SoftRender() { }

bool SoftRender::Initial(const int width, const int height) {
	_RenderDevice = std::make_unique<GdiDevice>(hwnd);
	if (!_RenderDevice->Initial(width, height))
	{
		std::cout << "Create Device Failed" << std::endl;
		return false;
	}

	//渲染层的视口
	this->width = width;
	this->height = height;
	this->NdcToScreen(0, 0) = (width - 1) * 0.5f;
	this->NdcToScreen(0, 3) = (width - 1)  * 0.5f;
	this->NdcToScreen(1, 1) = (height - 1) * 0.5f;
	this->NdcToScreen(1, 3) = (height - 1) * 0.5f;



	//test code
	this->texture.Load("C:\\Users\\Administrator\\Desktop\\first.png");
	GeometryGenerator::CreateBox(this->Mesh);

	return true;
}

void SoftRender::Tick() {

}

void SoftRender::Render() {
	_RenderDevice->ClearFrameBuffer();
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
	if (EdgVec0.x * EdgVec1.y - EdgVec0.y * EdgVec1.x >= 0.f)
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
	Out.Position = MainCamera->GetProjectMatrix() * MainCamera->GetViewMatrix() * vertex.Position;
	Out.PosH = 1.f / Out.Position.z;
	Out.UV = vertex.UV;
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
			//if (x1 >= 0 && x1 < this->width && y1 >= 0 && y1 < this->height)
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
			//if (x1 >= 0 && x1 < this->width && y1 >= 0 && y1 < this->height)
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
		if (MainCamera->GetCameraMode() == Camera::ENUM_ProjectType::Perspective) {
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
	auto preCross = [](const Vector2& p0, const Vector2& p1, const Vector2& p2) ->float {
		return (p1.x - p0.x)*(p2.y - p0.y) - (p2.x - p0.x)*(p1.y - p0.y);
	};
	//计算三角形的包围盒
	Vector2i MinXY, MaxXY;
	Vector2 ScreenPos0(v0.Position.x, v0.Position.y);
	Vector2 ScreenPos1(v1.Position.x, v1.Position.y);
	Vector2 ScreenPos2(v2.Position.x, v2.Position.y);
	float area = preCross(ScreenPos0, ScreenPos1, ScreenPos2);
	if (YGM::Math::Equal(area, 0.f)) {
		return;
	}
	MinXY.x = YGM::Math::Max(YGM::Math::Min(YGM::Math::Min(static_cast<int>(ScreenPos0.x), static_cast<int>(ScreenPos1.x)), static_cast<int>(ScreenPos2.x)), 0);
	MinXY.y = YGM::Math::Max(YGM::Math::Min(YGM::Math::Min(static_cast<int>(ScreenPos0.y), static_cast<int>(ScreenPos1.y)), static_cast<int>(ScreenPos2.y)), 0);
	MaxXY.x = YGM::Math::Min(YGM::Math::Max(YGM::Math::Max(static_cast<int>(ScreenPos0.x), static_cast<int>(ScreenPos1.x)), static_cast<int>(ScreenPos2.x)), this->width - 1);
	MaxXY.y = YGM::Math::Min(YGM::Math::Max(YGM::Math::Max(static_cast<int>(ScreenPos0.y), static_cast<int>(ScreenPos1.y)), static_cast<int>(ScreenPos2.y)), this->height - 1);

	for (int i = MinXY.x; i <= MaxXY.x; ++i) {
		for (int j = MinXY.y; j <= MaxXY.y; ++j) {
			Vector2 CurPoint(i, j);

			//另外一种判断点是否在三角形的方法：
			//使用两条边作为基线性表示对应点到目标的向量 只要满足 u + v <= 1 https://www.cnblogs.com/graphics/archive/2010/08/05/1793393.html

			//点满足在三边的内侧
			auto e2 = preCross(ScreenPos0, ScreenPos1, CurPoint);
			auto e0 = preCross(ScreenPos1, ScreenPos2, CurPoint);
			auto e1 = preCross(ScreenPos2, ScreenPos0, CurPoint);

			if (e0 >= 0.f && e1 >= 0.f && e2 >= 0.f) {
				//首先求当前点的z倒数： z = 1/(e1/z1 + e2/z2 + e3/z3);

				//计算插值系数
				float Cache0 = e0 / area * v0.PosH;
				float Cache1 = e1 / area * v1.PosH;
				float Cache2 = e2 / area * v2.PosH;
				float CurDepth = 1.f / (Cache0 + Cache1 + Cache2);

				//最后计算插值后的z
				float u = (v0.UV.u * Cache0 + v1.UV.u * Cache1 + v2.UV.u * Cache2) * CurDepth;
				float v = (v0.UV.v * Cache0 + v1.UV.v * Cache1 + v2.UV.v * Cache2) * CurDepth;
				_RenderDevice->DrawPixel(i, j, RGBAf(1.0f, 1.0f, 1.0f, 1.0f));
				//_RenderDevice->DrawPixel(i, j, texture.SampleNearest(u, 1.f - v));
			}
		}
	}
}