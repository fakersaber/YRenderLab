#ifndef _YRENDER_OPENGLRHI_GLFBO_H_
#define _YRENDER_OPENGLRHI_GLFBO_H_

#include <Public/OpenGLRHI/GLTexture.h>
#include <vector>

namespace YRender {
	//RenderTarget
	class GLFBO {
	public:
		enum class ENUM_TYPE : uint8_t{
			ENUM_TYPE_DYNAMIC_COLOR,
			ENUM_TYPE_COLOR_FLOAT
		};

		enum class TexRenderTarget {
			TEXTURE_2D,
			TEXTURE_CUBE_MAP_POSITIVE_X,
			TEXTURE_CUBE_MAP_NEGATIVE_X,
			TEXTURE_CUBE_MAP_POSITIVE_Y,
			TEXTURE_CUBE_MAP_NEGATIVE_Y,
			TEXTURE_CUBE_MAP_POSITIVE_Z,
			TEXTURE_CUBE_MAP_NEGATIVE_Z,
		};
		GLFBO();
		GLFBO(unsigned int width, unsigned int height, ENUM_TYPE type);
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

		//这里的weight与height只是renderbuffer的，不是framebuffer的，framebuffer需要指定视口的大小，即使用glviewport
		unsigned int width;
		unsigned int height;
	};
}





#endif