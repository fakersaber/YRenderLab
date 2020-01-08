#ifndef _YRENDER_SOFTRHI_H
#define _YRENDER_SOFTRHI_H

#include <Public/RenderClass.h>
#include <Public/SoftRHI/GdiDevice.h>

	class SoftRender : public RenderClass
	{
	public:
		SoftRender(const SoftRender& rhs) = delete;
		SoftRender& operator=(const SoftRender& rhs) = delete;
		SoftRender() = delete;
		explicit SoftRender(HWND hwnd);
		virtual ~SoftRender();

	public:
		virtual bool Initial(const int width, const int height) override;
		virtual void Tick() override;
		virtual void Render() override;

	private:
		void DrawIndexed(const MeshData& mesh);
		bool BackFaceCulling(const Vector4& v1, const Vector4& v2, const Vector4& v3);
		bool CVVClip(const Vector4& VertexPos);
		void PerspecDivision(Vector4& ClipPos);
		Vertex VertexShader(const Vertex& vertex);
		void NDCToScreen(Vector4& NdcVertex);
		void BresenhamDrawLine(int x1, int y1, int x2, int y2);


		//π‚’§À„∑®
		void HalfSpaceTriangle(const Vertex& v0,const Vertex& v1,const Vertex& v2);
	private:
		HWND hwnd;

		//test image
		Image texture;
	};



#endif