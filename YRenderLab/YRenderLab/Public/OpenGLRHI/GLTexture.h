#ifndef _YRENDER_OPENGLRHI_GLTEXTURE_H_
#define _YRENDER_OPENGLRHI_GLTEXTURE_H_

#include <vector>
#include <memory>


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

	enum class TexTureformat : uint8_t {
		TEX_GL_R32F, 
		TEX_GL_RG32F, 
		TEX_GL_RGB32F, 
		TEX_GL_RGBA32F
	};

public:
	GLTexture();
	GLTexture(unsigned int ID, ENUM_TYPE type);
	GLTexture(ENUM_TYPE type);
	GLTexture(std::shared_ptr<Image> img);
	GLTexture(const std::vector<std::string>& skybox);

	void Bind() const;
	void UnBind() const;
	bool GenMipmap();
	unsigned int Type2GL(ENUM_TYPE type) const;
	bool GenBufferForCubemap(unsigned int width, unsigned int height);
	bool IsValid() const;
	bool Use(unsigned int id);
	ENUM_TYPE GetType() const;
	unsigned int GetID() const;
	void Free();
private:
	bool SetImg(const Image& img);
	bool Load(const std::vector<std::string> & skybox);

private:
	unsigned int ID;
	ENUM_TYPE type;
};





#endif