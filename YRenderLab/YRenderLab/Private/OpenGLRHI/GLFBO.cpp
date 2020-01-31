#include <Public/OpenGLRHI/GLFBO.h>
#include <Public/OpenGLRHI/GLAD/glad/glad.h>
#include <iostream>

GLFBO::GLFBO()
	:isValid(false)
{ }


GLFBO::GLFBO(unsigned int width, unsigned int height, FBO_TYPE type)
	:
	width(width), height(height)
{
	//#TODO: Perform different logic based on type 
	switch (type) {
	case FBO_TYPE::ENUM_TYPE_DYNAMIC_COLOR:
		if (!GenFBO_DynamicColor(width, height))  printf("GenFBO_DynamicColor fail!\n");
		break;
	case FBO_TYPE::ENUM_TYPE_COLOR_FLOAT:
		if (!GenFBO_RGB16FColor(width, height))  printf("GenFBO_RGB16FColor fail!\n");
		break;
	}
}

GLFBO::GLFBO(unsigned int width, unsigned int height, const std::vector<GLTexture::TexTureformat>& VecForGbuffer)
	:
	width(width), height(height)
{
	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	unsigned int formats[4] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };
	unsigned int internalFormats[4] = { GL_R32F, GL_RG32F, GL_RGB32F, GL_RGBA32F };
	std::vector<unsigned int> attachments;

	for (int i = 0; i < VecForGbuffer.size(); i++) {

		unsigned int texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormats[static_cast<uint8_t>(VecForGbuffer[i])], width, height, 0, formats[static_cast<uint8_t>(VecForGbuffer[i])], GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texID, 0);

		attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
		colorTextures.emplace_back(texID, GLTexture::ENUM_TYPE_2D);
	}

	//Specifies a list of color buffers to be drawn into
	glDrawBuffers(static_cast<GLsizei>(VecForGbuffer.size()), attachments.data());

	isValid = IsComplete();
	if (!isValid) {
		printf("Framebuffer is not complete!\n");
		ID = 0;
		return;
	}

	return;
}





void GLFBO::SetRenderTargetToTexture(const GLTexture& tex, TexRenderTarget index, int mip) {
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
	if (idx == 0 && tex.GetType() != GLTexture::ENUM_TYPE_2D_DYNAMIC) {
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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (rst != GL_FRAMEBUFFER_COMPLETE) {
		printf("FrameBuffer is not complete!\n");
		return false;
	}
	return true;
}

//绑定的贴图一直会换，所以先创建一个renderbuffer上然后attach到depth
bool GLFBO::GenFBO_DynamicColor(unsigned int width, unsigned int height) {
	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	//just create renderbuffer and attach to depth buffer,not render
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
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

