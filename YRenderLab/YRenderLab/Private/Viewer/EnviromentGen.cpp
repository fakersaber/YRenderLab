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

	//先不实现IBL,走通cubemap
	//先draw模型，然后开启深度避免overdraw
	void EnviromentGen::InitGenShader_IBLSkybox(){
		//std::string vsPath = "data/shaders/IBL/cubemap.vs";
		//std::string fsPath = "data/shaders/IBL/equirectangular_to_cubemap.fs";
		//shader_genSkybox = GLShader(vsPath, fsPath);
		//shader_genSkybox.SetInt("equirectangularMap", 0);
		//auto captureProjection = YGM::Transform::Perspective(90.f, 1.0f, 0.1f, 10.0f);
		//shader_genSkybox.SetMat4f("projection", captureProjection);
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
		SkyBox = GLTexture(SkyboxTexturePath);
	}
}
