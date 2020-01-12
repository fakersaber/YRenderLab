#include <Public/Viewer/DeferredRaster.h>
#include <Public/Scene/Scene.h>
#include <Public/Viewer/EnviromentGen.h>
#include <Public/OpenGLRHI/GLFBO.h>
#include <Public/OpenGLRHI/GlfwWindow.h>
#include <Public/Scene/MeshComponent.h>
#include <Public/Scene/Yobject.h>
#include <Public/Scene/TransformComponent.h>
#include <Public/Basic/BSDF_StandardPBR.h>
#include <Public/Basic/MaterialComponent.h>
#include <Public/OpenGLRHI/GLAD/glad/glad.h>



DeferredRaster::DeferredRaster(std::shared_ptr<Scene> scene, std::shared_ptr<GlfwWindow> pGLWindow)
	:
	Raster(scene, New<EnviromentGen>(pGLWindow), pGLWindow)
{
}

void DeferredRaster::Draw(){
	glEnable(GL_DEPTH_TEST);

	//Update UBO
	{
		UpdateUBO_Camera();
		UpdateUBO_DirectionalLights();
		UpdateUBO_Environment();
	}


	//Update data that like shadows and lut etc...
	{
		enviromentGen->Visit(scene);
	}
	

	{
		Pass_GBuffer();
	}

}

void DeferredRaster::Initial(){
	Raster::Initial();

	
	InitShader_GBuffer();
	InitShader_AmbientLight();
	InitShader_Skybox();
	InitShader_TAA();
	InitShader_PostProcess();

	new (&gbufferFBO) 
		GLFBO(pGLWindow->GetViewPortW(), pGLWindow->GetViewPortH(), 
			{
				GLTexture::TexTureformat::TEX_GL_RGBA32F, //pixpos + roughness
				GLTexture::TexTureformat::TEX_GL_RGBA32F, //normal + metallic
				GLTexture::TexTureformat::TEX_GL_RGBA32F  //albedo + ao
			}
	);
}


void DeferredRaster::InitShader_GBuffer(){
	//Default material GBuffer
	{
		new (&GBuffer_StandardPBRShader) GLShader("Data/shaders/P3N3T2T3.vs", "Data/shaders/DeferredPipline/GBuffer_StandardPBR.fs");
		GBuffer_StandardPBRShader.SetInt("bsdf.albedoTexture", 0);
		GBuffer_StandardPBRShader.SetInt("bsdf.metallicTexture", 1);
		GBuffer_StandardPBRShader.SetInt("bsdf.roughnessTexture", 2);
		GBuffer_StandardPBRShader.SetInt("bsdf.aoTexture", 3);
		GBuffer_StandardPBRShader.SetInt("bsdf.normalTexture", 4);
		MapUBOToShader(GBuffer_StandardPBRShader);
	}

	//other material GBuffer
	{

	}
}

void DeferredRaster::InitShader_AmbientLight() {
}

void DeferredRaster::InitShader_Skybox(){
	new (&shader_skybox) GLShader("Data/shaders/SkyBox/skybox.vs", "Data/shaders/SkyBox/skybox.fs");

	shader_skybox.SetInt("skybox", 0);

	MapUBOToShader(shader_skybox);
}


void DeferredRaster::InitShader_Lights(){

}

void DeferredRaster::InitShader_TAA(){

}

void DeferredRaster::InitShader_PostProcess(){

}



void DeferredRaster::Pass_GBuffer(){
	gbufferFBO.Use();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Reset transform
	ObjectTransformVec.clear();
	ObjectTransformVec.emplace_back(1.f);

	this->Visit(scene->GetRoot());
}

void DeferredRaster::Pass_SkyBox()
{
}

void DeferredRaster::Pass_AmbientLight()
{
}

void DeferredRaster::Pass_Lights()
{
}

void DeferredRaster::Pass_TAA()
{
}

void DeferredRaster::Pass_PostProcess()
{
}



void DeferredRaster::Visit(std::shared_ptr<YObject> obj) {
	auto mesh = obj->GetComponent<MeshComponent>();
	auto material = obj->GetComponent<MaterialComponent>();
	auto children = obj->GetChildren();

	auto transform = obj->GetComponent<TransformComponent>();
	if (transform != nullptr) {
		//这里没有以父节点的transform为基
		ObjectTransformVec.push_back(/*modelVec.back() * */ transform->GetTransform());
	}

	if (material && material->GetMaterial() && mesh && mesh->GetMesh()) {
		this->Visit(Cast<BSDF_StandardPBR>(material->GetMaterial()));
		this->Visit(mesh->GetMesh());
	}

	for (auto child : children) {
		this->Visit(child);
	}

}



void DeferredRaster::Visit(std::shared_ptr<BSDF_StandardPBR> material) {
	SetCurShader(GBuffer_StandardPBRShader);
	std::string PreFix = "bsdf.";
	GBuffer_StandardPBRShader.SetVec3f(PreFix + "colorFactor", material->colorFactor);
	GBuffer_StandardPBRShader.SetFloat(PreFix + "metallicFactor", material->metallicFactor);
	GBuffer_StandardPBRShader.SetFloat(PreFix + "roughnessFactor", material->roughnessFactor);

	const int texNum = 5;
	std::shared_ptr<Image> imgs[texNum] = { material->albedoTexture, material->metallicTexture, material->roughnessTexture, material->aoTexture, material->normalTexture };
	for (int i = 0; i < texNum; ++i) {
		if (imgs[i] && imgs[i]->IsValid()) {
			pGLWindow->GetTexture(imgs[i]).Use(i);
		}
	}
}


void DeferredRaster::Visit(std::shared_ptr<TriMesh> mesh) {
	pGLWindow->GetVAO(mesh).Draw(curShader);
}