#ifndef _YRENDER_OPENGLRHI_GLRENDER_H
#define _YRENDER_OPENGLRHI_GLRENDER_H



#include <Public/RenderClass.h>
#include <Public/OpenGLRHI/GLFW/glfw3.h>
#include <Public/OpenGLRHI/GLAD/glad/glad.h>

namespace YRender {
	class GlRenderClass : public RenderClass
	{
	public:
		GlRenderClass(const GlRenderClass& rhs) = delete;
		GlRenderClass& operator=(const GlRenderClass& rhs) = delete;
		GlRenderClass() = delete;

		explicit GlRenderClass(GLFWwindow* window);
		virtual ~GlRenderClass();

	public:
		virtual bool Initial(const int width, const int height) override;
		virtual void Tick() override;
		virtual void Render() override;

	private:
		//void DrawIndexed(const MeshData& mesh);
		//bool BackFaceCulling(const Vector4& v1, const Vector4& v2, const Vector4& v3);
		//bool CVVClip(const Vector4& VertexPos);
		//void PerspecDivision(Vector4& ClipPos);
		//Vertex VertexShader(const Vertex& vertex);
		//void NDCToScreen(Vector4& NdcVertex);
		//void BresenhamDrawLine(int x1, int y1, int x2, int y2);


	private:

	};
}


#endif