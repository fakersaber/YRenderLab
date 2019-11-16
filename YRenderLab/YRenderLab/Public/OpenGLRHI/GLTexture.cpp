#include <Public/OpenGLRHI/GLTexture.h>
#include <Public/Basic/Image/Image.h>
#include <glad/glad.h>



namespace YRender {
	GLTexture::GLTexture() : ID(0), type(ENUM_TYPE_NOT_VALID) { }

	GLTexture::GLTexture(unsigned int ID, ENUM_TYPE type) : ID(ID), type(type) { }

	GLTexture::GLTexture(ENUM_TYPE type) : ID(0), type(type) {}

	GLTexture::GLTexture(std::shared_ptr<Image> img) : ID(0), type(ENUM_TYPE_2D_DYNAMIC) {
		SetImg(*img);
	}


	unsigned int GLTexture::Type2GL(ENUM_TYPE type) {
		switch (type)
		{
		case ENUM_TYPE_NOT_VALID:
			return 0;
		case ENUM_TYPE_2D:
		case ENUM_TYPE_2D_DYNAMIC:
			return GL_TEXTURE_2D;
		case ENUM_TYPE_CUBE_MAP:
			return GL_TEXTURE_CUBE_MAP;
		default:
			return 0;
		}
	}

	bool GLTexture::IsValid()
	{
		return ID != 0 && type != ENUM_TYPE_NOT_VALID;
	}

	bool GLTexture::SetImg(const Image& img) {
		if (type != ENUM_TYPE_2D_DYNAMIC) {
			printf("ERROR: type can't set image\n");
			return false;
		}

		if (!img.IsValid()) {
			printf("ERROR::Texture::SetImg:\n"
				"\t""img is invalid\n");
			return false;
		}

		if (ID == 0)
			glGenTextures(1, &ID);

		GLenum innerFormat, outerFormat;
		int nrComponents = img.GetChannel();
		if (nrComponents == 1) {
			innerFormat = GL_R16F;
			outerFormat = GL_RED;
		}
		else if (nrComponents == 3) {
			innerFormat = GL_RGB16F;
			outerFormat = GL_RGB;
		}
		else if (nrComponents == 4) {
			//GL_RGBA16F仅仅表示精度，每个像素大小被type（倒数第二个参数）指定
			innerFormat = GL_RGBA16F;
			outerFormat = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, innerFormat, img.GetWidth(), img.GetHeight(), 0, outerFormat, GL_FLOAT, reinterpret_cast<void*>(img.GetData()));
		glGenerateMipmap(GL_TEXTURE_2D); //内存多1/3
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		UnBind();

		return true;
	}

	void GLTexture::UnBind(){
		if (!IsValid())
			return;

		glBindTexture(Type2GL(type), 0);
	}


	bool GLTexture::Use(unsigned int id){
		if (!IsValid()) {
			printf("ERROR::Texture::Use:\n"
				"\t""use a invalid texture\n");
			return false;
		}

		glActiveTexture(GL_TEXTURE0 + id);
		glBindTexture(Type2GL(type), ID);
		return true;
	}
}