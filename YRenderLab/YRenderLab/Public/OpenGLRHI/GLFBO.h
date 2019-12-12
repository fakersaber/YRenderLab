#ifndef _YRENDER_OPENGLRHI_GLFBO_H_
#define _YRENDER_OPENGLRHI_GLFBO_H_

#include <Public/OpenGLRHI/GLTexture.h>
#include <vector>

namespace YRender {
	//RenderTarget
	class GLFBO {
	public:
		enum ENUM_TYPE {
			ENUM_TYPE_DYNAMIC_COLOR
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
		bool Use() const;
	private:
		bool IsComplete() const;
		

	private:
		unsigned int ID;
		std::vector<GLTexture> colorTexture;
		bool isValid;
		unsigned int width;
		unsigned int height;
	};
}





#endif