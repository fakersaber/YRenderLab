#include <Public/Viewer/EnviromentGen.h>
#include <Public/OpenGLRHI/GlfwWindow.h>
#include <Public/OpenGLRHI/GLAD/glad/glad.h>
#include <Public/Scene/Scene.h>
#include <Public/YGM/Transform.h>
#include <Public/Basic/Mesh/TriMesh.h>

#include <string>

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
	: skyboxSize(1024), irradianceSize(128), prefilterSize(1024), brdfSize(512), pGLWindow(pGLWindow), isInitBRDFFBO(false)
{
}

void EnviromentGen::Init() {
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	InitFBOs();
	InitShaders();
}


void EnviromentGen::Visit(std::shared_ptr<Scene> scene) {

	if (!scene || !scene->GetRoot()) {
		printf("ERROR::EnviromentGen::Visit(std::shared_ptr<Scene> scene):\n"
			"\t""scene or scene's root is nullptr\n");
		return;
	}
	auto environmentImg = scene->GetEnviromentImg();
	if (!environmentImg) {
		Clear();
		return;
	}

	//if img has changed
	if (curImg.lock() != environmentImg) {
		//save origin data 
		GLint origFBO;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &origFBO);
		GLint origViewport[4];
		glGetIntegerv(GL_VIEWPORT, origViewport);

		curImg = environmentImg;
		UpdateBRDFLut();
		UpdateSkyBox();
		UpdateIrradianceMap();
		UpdatePreFilterMap();
		glBindFramebuffer(GL_FRAMEBUFFER, origFBO);
		glViewport(origViewport[0], origViewport[1], origViewport[2], origViewport[3]);
	}
}


void EnviromentGen::InitShaders() {
	InitGenShader_IBLSkybox();
	InitGenShader_Irradiance();
	InitGenShader_PreFilter();
	InitGenShader_brdfLUT();
}

void EnviromentGen::InitFBOs() {
	genSkyboxFBO = GLFBO(skyboxSize, skyboxSize, GLFBO::FBO_TYPE::ENUM_TYPE_DYNAMIC_COLOR);
	genIrradianceFBO = GLFBO(irradianceSize, irradianceSize, GLFBO::FBO_TYPE::ENUM_TYPE_DYNAMIC_COLOR);
	for (int i = 0; i < maxMipLevels; i++) {
		int curSize = prefilterSize >> i;
		prefilterFBOs[i] = GLFBO(curSize, curSize, GLFBO::FBO_TYPE::ENUM_TYPE_DYNAMIC_COLOR);
	}
	genBrdfLutFBO = GLFBO(brdfSize, brdfSize, GLFBO::FBO_TYPE::ENUM_TYPE_COLOR_FLOAT);
}


//将equirectangular转换为cubemap
void EnviromentGen::InitGenShader_IBLSkybox() {
	std::string vsPath = "Data/shaders/IBL/cubeMap.vs";
	std::string fsPath = "Data/shaders/IBL/equirectangular_to_cubemap.fs";
	shader_genIBLSkybox = GLShader(vsPath, fsPath);
	shader_genIBLSkybox.SetInt("equirectangularMap", 0);
	auto captureProjection = YGM::Transform::Perspective(90.f, 1.0f, 0.1f, 10.0f);
	shader_genIBLSkybox.SetMat4f("projection", captureProjection.Transpose());
}


//生成漫反射的Irradiance
void EnviromentGen::InitGenShader_Irradiance() {
	std::string vsPath = "Data/shaders/IBL/cubeMap.vs";
	std::string fsPath = "Data/shaders/IBL/irradiance.fs";
	shader_genIrradianceMap = GLShader(vsPath, fsPath);
	shader_genIrradianceMap.SetInt("enviromentMap", 0);
	auto captureProjection = YGM::Transform::Perspective(90.f, 1.0f, 0.1f, 10.0f);
	shader_genIrradianceMap.SetMat4f("projection", captureProjection.Transpose());
}

void EnviromentGen::InitGenShader_PreFilter() {
	std::string vsPath = "Data/shaders/IBL/cubeMap.vs";
	std::string fsPath = "Data/shaders/IBL/prefilter.fs";
	shader_genPrefilterMap = GLShader(vsPath, fsPath);
	shader_genPrefilterMap.SetInt("environmentMap", 0);
	auto captureProjection = YGM::Transform::Perspective(90.f, 1.0f, 0.1f, 10.0f);
	shader_genPrefilterMap.SetMat4f("projection", captureProjection.Transpose());
}

void EnviromentGen::InitGenShader_brdfLUT() {
	std::string vsPath = "Data/shaders/P2T2.vs";
	std::string fsPath = "Data/shaders/IBL/brdf_lut.fs";
	shader_genBRDFLUT = GLShader(vsPath, fsPath);
}

void EnviromentGen::UpdateSkyBox() {
	skyBox = GLTexture(GLTexture::ENUM_TYPE_CUBE_MAP);
	skyBox.GenBufferForCubemap(skyboxSize, skyboxSize);
	genSkyboxFBO.Use();
	glViewport(0, 0, skyboxSize, skyboxSize);

	GLTexture imgTex(curImg.lock());
	imgTex.Use(0);

	for (int i = 0; i < 6; i++) {
		shader_genIBLSkybox.SetMat4f("view", captureViews[i].GetMatrix().Transpose());
		genSkyboxFBO.SetRenderTargetToTexture(skyBox, mapper[i]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		pGLWindow->GetVAO(TriMesh::OriginCube).Draw(shader_genIBLSkybox);
	}
	//{
	//	skyBox.Bind();
	//	auto TestMap = YRender::New<Image>(skyboxSize, skyboxSize, 3);
	//	for (int i = 0; i < 6; ++i) {
	//		glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, GL_FLOAT, TestMap->GetData());
	//		TestMap->SaveToPNG(std::string("C:/Users/Administrator/Desktop/YPipline/cubeMap") + std::to_string(i) + std::string(".png"));
	//	}
	//}
	skyBox.GenMipmap();
}

void EnviromentGen::UpdateIrradianceMap() {
	irradianceMap = GLTexture(GLTexture::ENUM_TYPE_CUBE_MAP);
	irradianceMap.GenBufferForCubemap(irradianceSize, irradianceSize);
	genIrradianceFBO.Use();
	glViewport(0, 0, irradianceSize, irradianceSize);

	skyBox.Use(0);
	for (int i = 0; i < 6; i++) {
		shader_genIrradianceMap.SetMat4f("view", captureViews[i].GetMatrix().Transpose());
		genIrradianceFBO.SetRenderTargetToTexture(irradianceMap, mapper[i]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		pGLWindow->GetVAO(TriMesh::OriginCube).Draw(shader_genIrradianceMap);
	}

	//{
	//	irradianceMap.Bind();
	//	auto TestMap = New<Image>(irradianceSize, irradianceSize, 3);
	//	for (int i = 0; i < 6; ++i) {
	//		glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, GL_FLOAT, TestMap->GetData());
	//		TestMap->SaveToPNG(std::string("C:/Users/Administrator/Desktop/YPipline/irradianceMap") + std::to_string(i) + std::string(".png"));
	//	}
	//}
}

void EnviromentGen::UpdatePreFilterMap() {
	prefilterMap = GLTexture(GLTexture::ENUM_TYPE_CUBE_MAP);
	prefilterMap.GenBufferForCubemap(prefilterSize, prefilterSize);
	prefilterMap.GenMipmap();

	shader_genPrefilterMap.SetFloat("resolution", static_cast<float>(prefilterSize));
	//5张prefilter，roughness为0.0  0.25  0.5  0.75  1.0 
	for (int mip = 0; mip < maxMipLevels; ++mip) {
		prefilterFBOs[mip].Use();
		skyBox.Use(0);

		// reisze framebuffer according to mip-level size.
		int curSize = prefilterSize >> mip;
		glViewport(0, 0, curSize, curSize);

		float roughness = static_cast<float>(mip) / (maxMipLevels - 1);
		shader_genPrefilterMap.SetFloat("roughness", roughness);
		for (int i = 0; i < 6; ++i) {
			shader_genPrefilterMap.SetMat4f("view", captureViews[i].GetMatrix().Transpose());
			prefilterFBOs[mip].SetRenderTargetToTexture(prefilterMap, mapper[i], mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			pGLWindow->GetVAO(TriMesh::OriginCube).Draw(shader_genPrefilterMap);
		}

		//{
		//	prefilterMap.Bind();
		//	auto TestMap = YRender::New<Image>(curSize, curSize, 3);
		//	for (int i = 0; i < 6; ++i) {
		//		glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mip, GL_RGB, GL_FLOAT, TestMap->GetData());
		//		TestMap->SaveToPNG(std::string("C:/Users/Administrator/Desktop/YPipline/prefilterMap") + 
		//			std::to_string(mip) + 
		//			"_" +
		//			std::to_string(i) + 
		//			std::string(".png"));
		//	}
		//}
	}
}

void EnviromentGen::UpdateBRDFLut() {
	if (isInitBRDFFBO)
		return;
	genBrdfLutFBO.Use();
	glViewport(0, 0, brdfSize, brdfSize);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pGLWindow->GetVAO(GlfwWindow::VAOTYPE::Screen).Draw(shader_genBRDFLUT);
	isInitBRDFFBO = true;

	//{
	//	genBrdfLutFBO.GetColorTexture(0).Bind();
	//	auto TestMap = New<Image>(brdfSize, brdfSize, 3);
	//	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, TestMap->GetData());
	//	TestMap->SaveToPNG(std::string("C:/Users/Administrator/Desktop/YPipline/brdf") + std::string(".png"),true);
	//}
}

void EnviromentGen::Clear() {
	curImg.reset();
	skyBox.Free();
	irradianceMap.Free();
	prefilterMap.Free();
}




//void EnviromentGen::InitSkyBoxTexture(){
	//std::vector<std::string> SkyboxTexturePath = 
	//{
	//	"Data/sky_box/right.jpg",
	//	"Data/sky_box/left.jpg",
	//	"Data/sky_box/top.jpg",
	//	"Data/sky_box/bottom.jpg",
	//	"Data/sky_box/front.jpg",
	//	"Data/sky_box/back.jpg"
	//};
	//OldSkyBox = std::make_shared<GLTexture>(SkyboxTexturePath);
//}
