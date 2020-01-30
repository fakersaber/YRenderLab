//#ifndef _YRENDER_OPENGLRHI_GLRENDER_H
//#define _YRENDER_OPENGLRHI_GLRENDER_H
//
//
//
//#include <Public/RenderClass.h>
//#include <Public/OpenGLRHI/GLFW/glfw3.h>
//#include <Public/OpenGLRHI/GLAD/glad/glad.h>
//
//
//
//class GLShader;
//class VAO;
//
//class GlRenderClass : public RenderClass
//{
//public:
//	GlRenderClass(const GlRenderClass& rhs) = delete;
//	GlRenderClass& operator=(const GlRenderClass& rhs) = delete;
//
//	GlRenderClass();
//	virtual ~GlRenderClass();
//
//public:
//	virtual bool Initial(const int width, const int height) override;
//	virtual void Tick() override;
//	virtual void Render() override;
//
//private:
//
//
//
//private:
//	std::unique_ptr<GLShader> GlslShader;
//	std::unique_ptr<VAO> GlVAO;
//};
//
//
//#endif