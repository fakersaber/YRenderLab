#include <Public/RenderClass.h>



namespace YRender {

	RenderClass* RenderClass::GetRender() {
		static RenderClass instance;
		return &instance;
	}

	RenderClass::RenderClass() {
		_RenderDevice = new RenderDevice();
	}

	bool RenderClass::Initial(HWND hwnd, const int width, const int height) {
		if (!_RenderDevice->Initial(hwnd, width, height))
		{
			std::cout << "Create Device Failed" << std::endl;
			return false;
		}
		GeometryGenerator::CreateBox(this->Mesh);
		return true;
	}

	RenderClass::~RenderClass() {
		delete _RenderDevice;
	}


	void RenderClass::Update() {

	}

	void RenderClass::Render() {
		//_RenderDevice->DrawPixel(400, 300);
		//_RenderDevice->DrawPixel(700, 500);
		//_RenderDevice->DrawFrameBuffer();
		DrawIndexed(this->Mesh);
	}


	bool RenderClass::BackFaceCulling(const Vertex& v1,const Vertex& v2,const Vertex& v3) {
		//以三角面指向观察者的方向为大拇指指向，其余手指逆时针绕着大拇指，如果手指的绕向和三角面顶点绕向一致，则这个面为正面，否则为背面。
		return true;
	}

	Vertex RenderClass::VertexShader(const Vertex& vertex) {
		Vertex Out;
		Out.Position = MainCamera.GetProjectMatrix() * MainCamera.GetViewMatrix() * vertex.Position;
		return Out;
	}


	void RenderClass::DrawIndexed(const MeshData& mesh) {
		for (auto i = 0; i < mesh.indices.size(); i+=3) {
			if (BackFaceCulling(mesh.vertices[mesh.indices[i]], mesh.vertices[mesh.indices[i + 1]], mesh.vertices[mesh.indices[i + 2]]))
				continue;
			
		}


	}

}