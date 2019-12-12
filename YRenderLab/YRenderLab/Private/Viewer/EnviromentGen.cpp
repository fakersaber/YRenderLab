#include <Public/Viewer/EnviromentGen.h>
#include <Public/OpenGLRHI/GlfwWindow.h>
#include <Public/OpenGLRHI/GLAD/glad/glad.h>
#include <Public/Scene/Scene.h>
#include <Public/YGM/Transform.h>
#include <Public/Basic/Mesh/TriMesh.h>

#include <string>

namespace YRender{
	const YGM::Transform EnviromentGen::captureViews[6] = {
		YGM::Transform::LookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f,  0.0f,  0.0f), Vector3(0.0f, -1.0f,  0.0f)),
		YGM::Transform::LookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3(-1.0f,  0.0f,  0.0f),Vector3(0.0f, -1.0f,  0.0f)),
		YGM::Transform::LookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f,  1.0f,  0.0f), Vector3(0.0f,  0.0f,  1.0f)),
		YGM::Transform::LookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f,  0.0f), Vector3(0.0f,  0.0f, -1.0f)),
		YGM::Transform::LookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f,  0.0f,  1.0f), Vector3(0.0f, -1.0f,  0.0f)),
		YGM::Transform::LookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f,  0.0f, -1.0f), Vector3(0.0f, -1.0f,  0.0f))
	};

	const GLFBO::TexRenderTarget EnviromentGen::mapper[6] = {
		GLFBO::TexRenderTarget::TEXTURE_CUBE_MAP_POSITIVE_X,
		GLFBO::TexRenderTarget::TEXTURE_CUBE_MAP_NEGATIVE_X,
		GLFBO::TexRenderTarget::TEXTURE_CUBE_MAP_POSITIVE_Y,
		GLFBO::TexRenderTarget::TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GLFBO::TexRenderTarget::TEXTURE_CUBE_MAP_POSITIVE_Z,
		GLFBO::TexRenderTarget::TEXTURE_CUBE_MAP_NEGATIVE_Z,
	};


	EnviromentGen::EnviromentGen(std::shared_ptr<GlfwWindow> pGLWindow)
		: skyboxSize(1024),irradianceSize(128), prefilterSize(1024), brdfSize(128), pGLWindow(pGLWindow)
	{
	}

	void EnviromentGen::Init(){
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		std::cout << glGetError() << std::endl;
		//old code
		//InitSkyBoxTexture();

		InitFBOs();
		InitShaders();
	}


	void EnviromentGen::Visit(std::shared_ptr<Scene> scene){
		//update IBL texture

		//test code 
		//if (curImg.expired()) {
			curImg = YRender::New<Image>("C:/Users/Administrator/Desktop/Arches_E_PineTree/Arches_E_PineTree_3k.hdr");
			UpdateSkyBox();
			UpdateIrradianceMap();
		//}
	}


	void EnviromentGen::InitShaders(){
		std::cout << glGetError() << std::endl;
		InitGenShader_IBLSkybox();
		InitGenShader_Irradiance();
	}

	void EnviromentGen::InitFBOs(){
		genSkyboxFBO = GLFBO(skyboxSize, skyboxSize, GLFBO::ENUM_TYPE_DYNAMIC_COLOR);
		genIrradianceFBO = GLFBO(irradianceSize, irradianceSize, GLFBO::ENUM_TYPE_DYNAMIC_COLOR);

	}


	//将equirectangular转换为cubemap
	void EnviromentGen::InitGenShader_IBLSkybox(){
		std::string vsPath = "data/shaders/IBL/cubeMap.vs";
		std::string fsPath = "data/shaders/IBL/equirectangular_to_cubemap.fs";
		shader_genIBLSkybox = GLShader(vsPath, fsPath);
		shader_genIBLSkybox.SetInt("equirectangularMap", 0);
		auto captureProjection = YGM::Transform::Perspective(90.f, 1.0f, 0.1f, 10.0f);
		shader_genIBLSkybox.SetMat4f("projection", captureProjection.GetMatrix().Transpose());
	}


	//生成漫反射的Irradiance
	void EnviromentGen::InitGenShader_Irradiance() {
		std::string vsPath = "data/shaders/IBL/cubeMap.vs";
		std::string fsPath = "data/shaders/IBL/irradiance.fs";
		shader_genIrradianceMap = GLShader(vsPath, fsPath);
		shader_genIrradianceMap.SetInt("enviromentMap", 0);
		auto captureProjection = YGM::Transform::Perspective(90.f, 1.0f, 0.1f, 10.0f);
		shader_genIrradianceMap.SetMat4f("projection", captureProjection.GetMatrix().Transpose());
	}

	void EnviromentGen::InitGenShader_PreFilter(){

	}

	void EnviromentGen::UpdateSkyBox(){
		skyBox = GLTexture(GLTexture::ENUM_TYPE_CUBE_MAP);
		skyBox.GenBufferForCubemap(skyboxSize, skyboxSize);
		genSkyboxFBO.Use();
		glViewport(0, 0, skyboxSize, skyboxSize);

		GLTexture imgTex(curImg);
		imgTex.Use(0);

		for (int i = 0; i < 6; i++) {
			shader_genIBLSkybox.SetMat4f("view", captureViews[i].GetMatrix().Transpose());
			genSkyboxFBO.SetRenderTargetToTexture(skyBox, mapper[i]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			pGLWindow->GetVAO(TriMesh::OriginCube).Draw(shader_genIBLSkybox);


		}
		{
			skyBox.Bind();
			auto TestMap = YRender::New<Image>(skyboxSize, skyboxSize, 3);
			for (int i = 0; i < 6; ++i) {
				glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, GL_FLOAT, TestMap->GetData());
				TestMap->SaveToPNG(std::string("C:/Users/Administrator/Desktop/YPipline/cubeMap") + std::to_string(i) + std::string(".png"));
			}
		}
		skyBox.GenMipmap();
	}

	void EnviromentGen::UpdateIrradianceMap() {
		irradianceMap = GLTexture(GLTexture::ENUM_TYPE_CUBE_MAP);
		irradianceMap.GenBufferForCubemap(irradianceSize, irradianceSize);
		//先用FBO渲染到Texture,RenderTarget设置为Texture,再调用glGetTexImage输出数据
		genIrradianceFBO.Use();
		glViewport(0, 0, irradianceSize, irradianceSize);

		skyBox.Use(0);
		for (int i = 0; i < 6; i++) {
			shader_genIrradianceMap.SetMat4f("view", captureViews[i].GetMatrix().Transpose());
			genIrradianceFBO.SetRenderTargetToTexture(irradianceMap, mapper[i]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			pGLWindow->GetVAO(TriMesh::OriginCube).Draw(shader_genIrradianceMap);
		}

		{
			irradianceMap.Bind();
			auto TestMap = YRender::New<Image>(irradianceSize, irradianceSize, 3);
			for (int i = 0; i < 6; ++i) {
				glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, GL_FLOAT, TestMap->GetData());
				TestMap->SaveToPNG(std::string("C:/Users/Administrator/Desktop/YPipline/irradianceMap") + std::to_string(i) + std::string(".png"));
			}
		}
	}


	void EnviromentGen::InitSkyBoxTexture(){
		std::vector<std::string> SkyboxTexturePath = 
		{
			"Data/sky_box/right.jpg",
			"Data/sky_box/left.jpg",
			"Data/sky_box/top.jpg",
			"Data/sky_box/bottom.jpg",
			"Data/sky_box/front.jpg",
			"Data/sky_box/back.jpg"
		};
		OldSkyBox = std::make_shared<GLTexture>(SkyboxTexturePath);
	}
}
