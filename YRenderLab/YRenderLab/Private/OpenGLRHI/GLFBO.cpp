#include <Public/OpenGLRHI/GLFBO.h>
#include <Public/OpenGLRHI/GLAD/glad/glad.h>
#include <Public/Basic/Image/Image.h>
#include <iostream>

GLFBO::GLFBO()
	:isValid(false)
{ }


GLFBO::GLFBO(unsigned int width, unsigned int height, FrameBufferType type)
	:
	width(width), height(height)
{
	switch (type) {
	case FrameBufferType::ENUM_TYPE_DYNAMIC_COLOR:
		if (!GenFBO_DynamicColor(width, height))
			printf("GenFBO_DynamicColor failed!\n");
		break;
	case FrameBufferType::ENUM_TYPE_COLOR_FLOAT:
		if (!GenFBO_RGB16FColor(width, height))
			printf("GenFBO_RGB16FColor failed!\n");
		break;
	case FrameBufferType::ENUM_TYPE_DEPTH:
		if (!GenFBO_Depth(width, height))
			printf("GenFBO_Depth failed!\n");
		break;
	default:
		printf("ERROR: FBO type not know\n");
		isValid = false;
		break;
	}
}

GLFBO::GLFBO(unsigned int width, unsigned int height, const std::vector<GLTexture::TexTureformat>& VecForGbuffer)
	:
	width(width), height(height), gbufferTypeVec(VecForGbuffer)
{
	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	//#TODO Mapping方式改变
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
		colorTextures.emplace_back(texID, GLTexture::ENUM_TYPE_2D_DYNAMIC);
	}

	//Specifies a list of color buffers to be drawn into
	glDrawBuffers(static_cast<GLsizei>(VecForGbuffer.size()), attachments.data());

	//FBO默认没有DEPTH！！！
	//make sure that your FBO has a depth attachment.
	//The depth buffer from your default framebuffer will not be used when rendering to an FBO. 
	//You need to create a depth renderbuffer, and attach it to the FBO.

	// create and attach depth buffer (renderbuffer)
	//实际上一般直接可以glFramebufferTexture2D绑定，创建RenderBuffer还是比较少用
	//这套代码就是瞎写..... 仅仅当初学习使用
	//每帧动态CopyDepth这种操作我都写得出来......
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);


	isValid = IsComplete();
	if (!isValid) {
		printf("Framebuffer is not complete!\n");
		ID = 0;
	}
}





void GLFBO::SetRenderTargetToTexture(const GLTexture& tex, RenderTargetType type, int mip) {
	if (!isValid) {
		printf("FrameBuffer is invalid\n");
		return;
	}

	//#TODO Mapping方式改变
	GLenum mapper[7] = {
		GL_TEXTURE_2D,
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	auto idx = static_cast<unsigned int>(type);
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

GLTexture GLFBO::GetColorTexture(unsigned int index) const{
	GLTexture texture;
	if (isValid && index < colorTextures.size())
		texture = colorTextures[index];
	return texture;
}

GLTexture GLFBO::GetDepthTexture() const{
	return depthTexture;
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
	//std::cout<<glGetError()<<std::endl;
	if (rst != GL_FRAMEBUFFER_COMPLETE) {
		printf("FrameBuffer is not complete!\n");
		return false;
	}
	return true;
}

bool GLFBO::GenFBO_DynamicColor(unsigned int width, unsigned int height) {
	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	//create and attach depth buffer (renderbuffer)
	unsigned int depthbuffer;
	glGenRenderbuffers(1, &depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

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
	//没有attach depth
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

	colorTextures.emplace_back(colorBufferID, GLTexture::ENUM_TYPE_2D_DYNAMIC);
	return true;
}

bool GLFBO::GenFBO_Depth(unsigned int width, unsigned int height) {
	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	// create depth texture
	unsigned int depthTextureId;
	glGenTextures(1, &depthTextureId);
	glBindTexture(GL_TEXTURE_2D, depthTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); //clamp的颜色为border颜色
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureId, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	isValid = IsComplete();
	if (!isValid) {
		printf("Framebuffer is not complete!\n");
		//显存上的资源并没有被释放
		colorTextures.clear();
		return false;
	}
	new (&depthTexture) GLTexture(depthTextureId, GLTexture::ENUM_TYPE_2D_DYNAMIC);
	return true;
}



void GLFBO::CopyFrameBuffer(const GLFBO& DesFBO, const GLFBO& SrcFBO, RenderTargetCopyType type) {
	if (!DesFBO.isValid || !SrcFBO.isValid)
		return;

	unsigned int CopyType[] = { GL_COLOR_BUFFER_BIT ,GL_DEPTH_BUFFER_BIT };

	auto index = static_cast<unsigned int>(type);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, SrcFBO.ID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, DesFBO.ID);
	glBlitFramebuffer(0, 0, SrcFBO.width, SrcFBO.height, 0, 0, DesFBO.width, DesFBO.height, CopyType[index], GL_NEAREST);
}


void GLFBO::UseDefault() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void GLFBO::DebugOutPutFrameBuffer(const GLFBO& DebugFBO, DebugType type) {
	DebugFBO.Use();
	switch (type) {
	case DebugType::DebugType_Color: {
		auto TestMap = New<Image>(DebugFBO.width, DebugFBO.height, 3);
		for (int i = 0; i < DebugFBO.colorTextures.size(); ++i) {
			DebugFBO.GetColorTexture(i).Bind();
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, TestMap->GetData());
			TestMap->SaveToPNG(std::string("C:/Users/Administrator/Desktop/YPipline/") + std::to_string(i) + std::string(".png"), true);
		}
		break;
	}
	case DebugType::DebugType_Depth: {
		auto TestMap = New<Image>(DebugFBO.width, DebugFBO.height, 1);
		DebugFBO.depthTexture.Bind();
		glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, TestMap->GetData());
		TestMap->SaveToPNG(std::string("depth.png"), true);
		break;
	}
	}
}

void GLFBO::Resize(unsigned int width, unsigned int height) {
	if (!isValid)
		return;
	//if (type != ENUM_TYPE_GBUFFER)
	//	return;
	if (this->width == width && this->height == height)
		return;

	if (width == 0 || height == 0)
		return;

	this->width = width;
	this->height = height;

	//FBO Free 还是资源管理的问题
	{
		for (auto colorTex : colorTextures)
			colorTex.Free();
		colorTextures.clear();
		//depthTexture.Free();
		glDeleteFramebuffers(1, &ID);
	}



	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	//#TODO Mapping方式改变
	unsigned int formats[4] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };
	unsigned int internalFormats[4] = { GL_R32F, GL_RG32F, GL_RGB32F, GL_RGBA32F };
	std::vector<unsigned int> attachments;

	for (int i = 0; i < gbufferTypeVec.size(); i++) {

		unsigned int texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormats[static_cast<uint8_t>(gbufferTypeVec[i])], width, height, 0, formats[static_cast<uint8_t>(gbufferTypeVec[i])], GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texID, 0);

		attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
		colorTextures.emplace_back(texID, GLTexture::ENUM_TYPE_2D_DYNAMIC);
	}

	//Specifies a list of color buffers to be drawn into
	glDrawBuffers(static_cast<GLsizei>(gbufferTypeVec.size()), attachments.data());

	//FBO默认没有DEPTH！！！
	//make sure that your FBO has a depth attachment.
	//The depth buffer from your default framebuffer will not be used when rendering to an FBO. 
	//You need to create a depth renderbuffer, and attach it to the FBO.

	// create and attach depth buffer (renderbuffer)
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);


	isValid = IsComplete();
	if (!isValid) {
		printf("Framebuffer is not complete!\n");
		ID = 0;
	}

}