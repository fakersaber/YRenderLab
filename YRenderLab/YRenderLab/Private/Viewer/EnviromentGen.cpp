#include <Public/Viewer/EnviromentGen.h>
#include <Public/OpenGLRHI/GLAD/glad/glad.h>
#include <Public/Scene/Scene.h>
#include <Public/YGM/Transform.h>
#include <string>

namespace YRender{
	void EnviromentGen::Init(){
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		InitSkyBoxTexture();

		InitShaders();
	}


	void EnviromentGen::Visit(std::shared_ptr<Scene> scene){
		//update IBL texture
	}


	void EnviromentGen::InitShaders(){
		InitGenShader_IBLSkybox();
	}


	//将equirectangular转换为cubemap
	void EnviromentGen::InitGenShader_IBLSkybox(){
		std::string vsPath = "data/shaders/P3N3T2.vs";
		std::string fsPath = "data/shaders/IBL/equirectangular_to_cubemap.fs";
		shader_genIBLSkybox = GLShader(vsPath, fsPath);
		shader_genIBLSkybox.SetInt("equirectangularMap", 0);
		//auto captureProjection = YGM::Transform::Perspective(90.f, 1.0f, 0.1f, 10.0f);
		//shader_genIBLSkybox.SetMat4f("projection", captureProjection.GetMatrix().Transpose());
	}


	//生成漫反射的Irradiance
	void EnviromentGen::InitGenShader_Irradiance() {

	}

	void EnviromentGen::InitGenShader_PreFilter(){

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
		SkyBox = std::make_shared<GLTexture>(SkyboxTexturePath);
	}
}
