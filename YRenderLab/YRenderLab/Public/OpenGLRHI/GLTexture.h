#ifndef _YRENDER_OPENGLRHI_GLTEXTURE_H_
#define _YRENDER_OPENGLRHI_GLTEXTURE_H_

#include <vector>
#include <memory>

namespace YRender {
	class Image;
	class GLTexture {
	public:
		enum ENUM_TYPE
		{
			ENUM_TYPE_NOT_VALID,
			ENUM_TYPE_2D,
			ENUM_TYPE_CUBE_MAP,
			ENUM_TYPE_2D_DYNAMIC,
		};
	public:
		GLTexture();
		GLTexture(unsigned int ID, ENUM_TYPE type);
		GLTexture(ENUM_TYPE type);
		GLTexture(std::shared_ptr<Image> img);
		void UnBind();
		unsigned int Type2GL(ENUM_TYPE type);
		bool IsValid();
		bool Use(unsigned int id);
	private:
		bool SetImg(const Image& img);

	private:
		unsigned int ID;
		ENUM_TYPE type;
	};
}





#endif