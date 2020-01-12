#ifndef _YRENDER_OPENGLRHI_GLFBO_H_
#define _YRENDER_OPENGLRHI_GLFBO_H_

#include <Public/OpenGLRHI/GLTexture.h>
#include <vector>


//texture与renderbuffer都属于rendertarget，而一个framebufferobject必须绑定rendertarget
//renderbuffer只能渲染到viewport上，所以作用不如texture，但由于其是内置变量，速度会快一些
class GLFBO {
public:
	enum class FBO_TYPE : uint8_t {
		ENUM_TYPE_DYNAMIC_COLOR,
		ENUM_TYPE_COLOR_FLOAT,

	};

	enum class TexRenderTarget : uint8_t {
		TEXTURE_2D,
		TEXTURE_CUBE_MAP_POSITIVE_X,
		TEXTURE_CUBE_MAP_NEGATIVE_X,
		TEXTURE_CUBE_MAP_POSITIVE_Y,
		TEXTURE_CUBE_MAP_NEGATIVE_Y,
		TEXTURE_CUBE_MAP_POSITIVE_Z,
		TEXTURE_CUBE_MAP_NEGATIVE_Z,
	};


	GLFBO();
	GLFBO(unsigned int width, unsigned int height, FBO_TYPE type);
	GLFBO(unsigned int width, unsigned int height, const std::vector<GLTexture::TexTureformat>& VecForGbuffer); //绑定多个rendertarget
	void SetRenderTargetToTexture(const GLTexture& tex, TexRenderTarget index, int mip = 0);
	GLTexture GetColorTexture(unsigned int index) const;
	bool Use() const;
private:
	bool IsComplete() const;
	bool GenFBO_DynamicColor(unsigned int width, unsigned int height);
	bool GenFBO_RGB16FColor(unsigned int width, unsigned int height);
private:
	unsigned int ID;
	std::vector<GLTexture> colorTextures;
	bool isValid;

	unsigned int width;
	unsigned int height;
};






#endif