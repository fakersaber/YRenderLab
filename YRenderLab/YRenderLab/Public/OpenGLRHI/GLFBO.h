#ifndef _YRENDER_OPENGLRHI_GLFBO_H_
#define _YRENDER_OPENGLRHI_GLFBO_H_

#include <Public/OpenGLRHI/GLTexture.h>
#include <vector>


//texture与renderbuffer都属于rendertarget，而一个framebufferobject必须绑定rendertarget
//renderbuffer只能渲染到viewport上，所以作用不如texture，但由于其是内置变量，速度会快一些
class GLFBO {
public:
	enum class FrameBufferType : uint8_t {
		ENUM_TYPE_DYNAMIC_COLOR,   //动态变换的Framebuffer，仅仅create depth
		ENUM_TYPE_COLOR_FLOAT,     //16f精度的rgb，没有深度
		ENUM_TYPE_DEPTH,           //只绑定深度的Framebuffer,设置gldrawbuffer为GL_NONE不会draw任何color
	};

	enum class RenderTargetType : uint8_t {
		TEXTURE_2D,
		TEXTURE_CUBE_MAP_POSITIVE_X,
		TEXTURE_CUBE_MAP_NEGATIVE_X,
		TEXTURE_CUBE_MAP_POSITIVE_Y,
		TEXTURE_CUBE_MAP_NEGATIVE_Y,
		TEXTURE_CUBE_MAP_POSITIVE_Z,
		TEXTURE_CUBE_MAP_NEGATIVE_Z,
	};

	enum class RenderTargetCopyType : uint8_t {
		COPY_COLOR_BUFFER,
		COPY_DEPTH_BUFFER,
	};
	
	static void UseDefault();
	static void CopyFrameBuffer(const GLFBO& DesFBO, const GLFBO& SrcFBO, RenderTargetCopyType type);
	static void DebugOutPutFrameBuffer(const GLFBO& DebugFBO);
	GLFBO();
	GLFBO(unsigned int width, unsigned int height, FrameBufferType type);
	GLFBO(unsigned int width, unsigned int height, const std::vector<GLTexture::TexTureformat>& VecForGbuffer); //绑定多个rendertarget
	void SetRenderTargetToTexture(const GLTexture& tex, RenderTargetType type, int mip = 0);
	GLTexture GetColorTexture(unsigned int index) const;
	bool Use() const;
	void Resize(unsigned int width, unsigned int height);
private:
	bool IsComplete() const;
	bool GenFBO_DynamicColor(unsigned int width, unsigned int height);
	bool GenFBO_RGB16FColor(unsigned int width, unsigned int height);
	bool GenFBO_Depth(unsigned int width, unsigned int height);
private:
	unsigned int ID;
	std::vector<GLTexture> colorTextures;
	GLTexture depthTexture;
	bool isValid;
	std::vector<GLTexture::TexTureformat> gbufferTypeVec;
	unsigned int width;
	unsigned int height;
};






#endif