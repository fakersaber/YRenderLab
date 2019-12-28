#include <Public/OpenGLRHI/GLFBO.h>
#include <Public/OpenGLRHI/GLAD/glad/glad.h>
#include <iostream>

namespace YRender {

	GLFBO::GLFBO()
		:isValid(false) 
	{ }


	GLFBO::GLFBO(unsigned int width, unsigned int height, ENUM_TYPE type)
	:
		width(width),height(height)
	{
		//todo: Perform different logic based on type 
		switch (type) {
		case ENUM_TYPE::ENUM_TYPE_DYNAMIC_COLOR:
			if (!GenFBO_DynamicColor(width, height))  printf("GenFBO_DynamicColor fail!\n");
			break;
		case ENUM_TYPE::ENUM_TYPE_COLOR_FLOAT:
			if (!GenFBO_RGB16FColor(width, height))  printf("GenFBO_RGB16FColor fail!\n");
			break;
		}



	}

	void GLFBO::SetRenderTargetToTexture(const GLTexture& tex, TexRenderTarget index, int mip){
		if (!isValid) {
			printf("FrameBuffer is invalid\n");
			return;
		}

		GLenum mapper[7] = {
			GL_TEXTURE_2D,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		};

		auto idx = static_cast<unsigned int>(index);
		if(idx == 0 && tex.GetType() != GLTexture::ENUM_TYPE_2D_DYNAMIC){
			printf("SetRenderTarget Error! the texture is not support GL_TEXTURE_2D");
			return;
		}

		if (idx > 0 && tex.GetType() != GLTexture::ENUM_TYPE_CUBE_MAP) {
			printf("Set RenderTarget Error! the texture is not support GL_TEXTURE_CUBE_MAP");
			return;
		}

		this->Use();

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mapper[idx], tex.GetID(), mip);
	}

	GLTexture GLFBO::GetColorTexture(unsigned int index) const
	{
		GLTexture texture;
		if (isValid && index < colorTextures.size())
			texture = colorTextures[index];
		return texture;
	}


	bool GLFBO::Use() const {
		if (!isValid) {
			printf("ERROR::FBO::Use:\n"
				"\t""FBO is invalid\n");
			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, ID);
		return true;
	}


	bool GLFBO::IsComplete() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
		auto rst = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		if (rst != GL_FRAMEBUFFER_COMPLETE) {
			printf("FrameBuffer is not complete!\n");
			return false;
		}
		return true;
	}


	bool GLFBO::GenFBO_DynamicColor(unsigned int width, unsigned int height){
		glGenFramebuffers(1, &ID);
		glBindFramebuffer(GL_FRAMEBUFFER, ID);

		//just create renderbuffer and attach to depth buffer,not render
		//这里只是一个创建renderbuffer然后绑定的过程，并没有向framebuffer渲染东西
		unsigned int renderbuffer;
		glGenRenderbuffers(1, &renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		isValid = IsComplete();
		if (!isValid) {
			printf("Framebuffer is not complete!\n");
			colorTextures.clear();
			return false;
		}
		return true;
	}


	bool GLFBO::GenFBO_RGB16FColor(unsigned int width, unsigned int height)
	{
		glGenFramebuffers(1, &ID);
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
		// create a color attachment texture
		unsigned int colorBufferID;
		glGenTextures(1, &colorBufferID);
		glBindTexture(GL_TEXTURE_2D, colorBufferID);
		glTexImage2D(GL_TEXTURE_2D,0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferID, 0);	// we only need a color buffer

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		isValid = IsComplete();
		if (!isValid) {
			printf("Framebuffer is not complete!\n");
			//显存上的资源并没有被释放
			colorTextures.clear();
			return false;
		}

		colorTextures.emplace_back(colorBufferID, GLTexture::ENUM_TYPE_2D);
		return true;
	}
}

