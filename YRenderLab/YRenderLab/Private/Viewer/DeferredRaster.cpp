#include <Public/Viewer/DeferredRaster.h>
#include <Public/Scene/Scene.h>

#include <Public/Viewer/EnviromentGen.h>
#include <Public/Viewer/ShadowGen.h>

#include <Public/OpenGLRHI/GLFBO.h>
#include <Public/OpenGLRHI/GlfwWindow.h>

#include <Public/Scene/MeshComponent.h>
#include <Public/Basic/Mesh/Primitive.h>


#include <Public/Scene/Yobject.h>
#include <Public/Scene/TransformComponent.h>

#include <Public/Basic/BSDF_Emission.h>
#include <Public/Basic/BSDF_StandardPBR.h>
#include <Public/Basic/MaterialComponent.h>


#include <Public/OpenGLRHI/GLAD/glad/glad.h>
#include <Public/Scene/LightComponent.h>
#include <Public/Lights/DirectionalLight.h>
#include <Public/Lights/PointLight.h>

DeferredRaster::DeferredRaster(std::shared_ptr<Scene> scene, std::shared_ptr<GlfwWindow> pGLWindow)
	:
	Raster(scene, 
		New<EnviromentGen>(pGLWindow), 
		New<ShadowGen>(pGLWindow),
		pGLWindow
	)
{
}

void DeferredRaster::Resize(unsigned int width, unsigned int height) {
	gbufferFBO.Resize(width, height);
	windowFBO.Resize(width, height);
}

void DeferredRaster::Draw() {
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//can't use UBO
	{
		enviromentGen->UpdateEnvironment(scene);
		shadowGen->UpdateShadowMap(scene);
	}


	//Update UBO
	{
		UpdateUBO_Camera();
		UpdateUBO_DirectionalLights();
		UpdateUBO_Environment();
	}


	{
		Pass_GBuffer();

		glDisable(GL_DEPTH_TEST);
		Pass_Lights();
		Pass_AmbientLight();
		glEnable(GL_DEPTH_TEST);

		//渲染完opqueue后再渲染Transparent
		//Pass_Transparent();

		Pass_SkyBox();

		//Pass_TAA();

		Pass_PostProcess();
	}

}

void DeferredRaster::Initial() {
	Raster::Initial();

	InitShader_GBuffer();
	InitShader_Lights();
	InitShader_AmbientLight();
	InitShader_Skybox();
	InitShader_TAA();
	InitShader_PostProcess();

	new (&gbufferFBO)
		GLFBO(
			pGLWindow->GetViewPortW(), 
			pGLWindow->GetViewPortH(),
			{
				GLTexture::TexTureformat::TEX_GL_RGBA32F, //pixpos + roughness
				GLTexture::TexTureformat::TEX_GL_RGBA32F, //normal + metallic
				GLTexture::TexTureformat::TEX_GL_RGBA32F,  //albedo + ao
				GLTexture::TexTureformat::TEX_GL_RGBA32F // ID
			}
	);

	new (&windowFBO)
		GLFBO(
			pGLWindow->GetViewPortW(),
			pGLWindow->GetViewPortH(),
			{
				GLTexture::TexTureformat::TEX_GL_RGB32F
			}
	);

}


void DeferredRaster::InitShader_GBuffer() {
	//Default material GBuffer
	{
		new (&GBuffer_StandardPBRShader) GLShader("Data/shaders/P3N3T2T3.vs", "Data/shaders/DeferredPipline/GBuffer_StandardPBR.fs");
		GBuffer_StandardPBRShader.SetInt("Material.albedoTexture",0);
		GBuffer_StandardPBRShader.SetInt("Material.metallicTexture", 1);
		GBuffer_StandardPBRShader.SetInt("Material.roughnessTexture", 2);
		GBuffer_StandardPBRShader.SetInt("Material.aoTexture", 3);
		GBuffer_StandardPBRShader.SetInt("Material.normalTexture", 4);

		GBuffer_StandardPBRShader.SetInt("ID", static_cast<int>(CoreDefine::MaterialID::BSDF_StandardPBR));

		MapUBOToShader(GBuffer_StandardPBRShader);
	}


	//Eimission Material Gbuffer
	{
		new (&GBuffer_EmissionShader) GLShader("Data/shaders/P3N3T2.vs", "Data/shaders/DeferredPipline/GBuffer_Emission.fs");

		GBuffer_EmissionShader.SetInt("ID", static_cast<int>(CoreDefine::MaterialID::BSDF_Emission));

		MapUBOToShader(GBuffer_EmissionShader);
	}
}

void DeferredRaster::InitShader_Lights() {
	new (&DirectLight_Shader) GLShader("Data/shaders/P2T2.vs", "Data/shaders/DeferredPipline/DirectLight.fs");
	//int StartIndex = 0;
	DirectLight_Shader.SetInt("GBuffer0", 0);
	DirectLight_Shader.SetInt("GBuffer1", 1);
	DirectLight_Shader.SetInt("GBuffer2", 2);
	DirectLight_Shader.SetInt("GBuffer3", 3);


	int LightStartIndex = 4;

	for (int i = 0; i < CoreDefine::maxDirectionalLights; ++i) {
		DirectLight_Shader.SetInt("directionalLightDepthMap0", LightStartIndex++);
	}

	for (int i = 0; i < CoreDefine::maxPointLights; ++i) {

	}

	for (int i = 0; i < CoreDefine::maxSpotLights; ++i) {

	}

	MapUBOToShader(DirectLight_Shader);
}

void DeferredRaster::InitShader_AmbientLight() {
	new (&AmbientLight_Shader) GLShader("Data/shaders/P2T2.vs", "Data/shaders/DeferredPipline/AmbientLight.fs");
	
	int startIndex = 0;
	AmbientLight_Shader.SetInt("GBuffer0", startIndex++);
	AmbientLight_Shader.SetInt("GBuffer1", startIndex++);
	AmbientLight_Shader.SetInt("GBuffer2", startIndex++);
	AmbientLight_Shader.SetInt("GBuffer3", startIndex++);

	AmbientLight_Shader.SetInt("irradianceMap", startIndex++);
	AmbientLight_Shader.SetInt("prefilterMap", startIndex++);
	AmbientLight_Shader.SetInt("brdfLUT", startIndex++);


	MapUBOToShader(AmbientLight_Shader);
}

void DeferredRaster::InitShader_Skybox() {
	new (&Skybox_Shader) GLShader("Data/shaders/SkyBox/skybox.vs", "Data/shaders/SkyBox/skybox.fs");
	Skybox_Shader.SetInt("skybox", 0);
	MapUBOToShader(Skybox_Shader);
}


void DeferredRaster::InitShader_TAA() {

}

void DeferredRaster::InitShader_PostProcess() {
	new (&postProcessShader) GLShader("Data/shaders/P2T2.vs", "Data/shaders/DeferredPipline/PostProcess.fs");
	postProcessShader.SetInt("SceneBuffer", 0);
	//暂时无需uniform block 所以不需要映射
	//MapUBOToShader(postProcessShader)
}



void DeferredRaster::Pass_GBuffer() {
	gbufferFBO.Use();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	this->RenderScene(scene->GetRoot());
}

void DeferredRaster::Pass_Lights() {
	windowFBO.Use();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// set gbuffer
	gbufferFBO.GetColorTexture(0).Use(0);
	gbufferFBO.GetColorTexture(1).Use(1);
	gbufferFBO.GetColorTexture(2).Use(2);
	gbufferFBO.GetColorTexture(3).Use(3);

	int DirectinalLightIndex = 4;
	int PointLightIndex = 0;
	int SpotLightIndex = 0;

	for (auto& lightComponent : scene->GetLightComponents()) {
		auto light = lightComponent->GetLight();
		if (auto directinalLight = Cast<DirectionalLight>(light)) {
			shadowGen->GetDirectionalLightShadowMap(lightComponent).Use(DirectinalLightIndex++);
			continue;
		}
	}


	pGLWindow->GetVAO(CoreDefine::StaticVAOType::Screen).Draw(DirectLight_Shader);
}



//两个计算光照的Pass都不用开depth，因为前面的pass已经算过一次深度信息了
void DeferredRaster::Pass_AmbientLight() {
	//环境光需要叠加解析光
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	windowFBO.Use();


	gbufferFBO.GetColorTexture(0).Use(0);
	gbufferFBO.GetColorTexture(1).Use(1);
	gbufferFBO.GetColorTexture(2).Use(2);
	gbufferFBO.GetColorTexture(3).Use(3);

	const int environmentBase = 4;

	auto environment = scene->GetEnviromentImg();
	if (environment) {
		auto irradianceMap = enviromentGen->GetIrradianceMap();
		auto prefilterMap = enviromentGen->GetPrefilterMap();	
		auto brdfLUT = enviromentGen->GetBRDF_LUT();

		irradianceMap.Use(environmentBase);
		prefilterMap.Use(environmentBase + 1);
		brdfLUT.Use(environmentBase + 2);
	}

	pGLWindow->GetVAO(CoreDefine::StaticVAOType::Screen).Draw(AmbientLight_Shader);
	glDisable(GL_BLEND);
}


void DeferredRaster::Pass_SkyBox() {
	GLFBO::CopyFrameBuffer(windowFBO,gbufferFBO,GLFBO::RenderTargetCopyType::COPY_DEPTH_BUFFER);
	windowFBO.Use();
	if(auto environment = scene->GetEnviromentImg()){
		enviromentGen->GetSkyBox().Use(0);
	}
	glDepthFunc(GL_LEQUAL);
	pGLWindow->GetVAO(CoreDefine::StaticVAOType::Cube).Draw(Skybox_Shader);
	glDepthFunc(GL_LESS);
}

void DeferredRaster::Pass_TAA() {

}

void DeferredRaster::Pass_PostProcess(){
	GLFBO::UseDefault();
	windowFBO.GetColorTexture(0).Use(0);
	pGLWindow->GetVAO(CoreDefine::StaticVAOType::Screen).Draw(postProcessShader);

}



void DeferredRaster::RenderScene(std::shared_ptr<YObject> obj) {
	auto mesh = obj->GetComponent<MeshComponent>();
	auto material = obj->GetComponent<MaterialComponent>();
	auto transform = obj->GetComponent<TransformComponent>();

	if (material && material->GetMaterial() && mesh && mesh->GetPrimitive() && transform) {

		material->GetMaterial()->SetCurMaterial(shared_this<Raster>());

		mesh->GetPrimitive()->RenderPrimitive(shared_this<Raster>(), transform->GetWorldTransform());
	}
	
	for (auto child : obj->GetChildrens()) {
		this->RenderScene(child);
	}

}


void DeferredRaster::SetMaterial(std::shared_ptr<BSDF_StandardPBR> material) {

	SetCurShader(GBuffer_StandardPBRShader);
	GBuffer_StandardPBRShader.SetVec3f("Material.colorFactor", material->colorFactor);
	GBuffer_StandardPBRShader.SetFloat("Material.metallicFactor", material->metallicFactor);
	GBuffer_StandardPBRShader.SetFloat("Material.roughnessFactor", material->roughnessFactor);

	const int texNum = 5;
	std::shared_ptr<Image> imgs[texNum] = {
		material->albedoTexture,
		material->metallicTexture,
		material->roughnessTexture,
		material->aoTexture,
		material->normalTexture
	};


	//因为在这里Bind只有初始化创建Texture才调用，所以只有第一帧才有Texture的问题
	//在调用到下一个glActiveTexture前Bind到错误的texture
	auto _Texture0 = pGLWindow->GetTexture(imgs[0]);
	auto _Texture1 = pGLWindow->GetTexture(imgs[1]);
	auto _Texture2 = pGLWindow->GetTexture(imgs[2]);
	auto _Texture3 = pGLWindow->GetTexture(imgs[3]);
	auto _Texture4 = pGLWindow->GetTexture(imgs[4]);

	_Texture0.Use(0);
	_Texture1.Use(1);
	_Texture2.Use(2);
	_Texture3.Use(3);
	_Texture4.Use(4);
}


void DeferredRaster::SetMaterial(std::shared_ptr<BSDF_Emission> EmissionMaterial) {

	SetCurShader(GBuffer_EmissionShader);

	GBuffer_EmissionShader.SetVec3f("emission.L", EmissionMaterial->color * EmissionMaterial->intensity);
}



void DeferredRaster::RenderMesh(std::shared_ptr<TriMesh> mesh, const YGM::Transform& model) {
	curShader.SetMat4f("model", model.GetMatrix().Transpose());
	pGLWindow->GetVAO(mesh).Draw(curShader);
}

void DeferredRaster::RenderMesh(std::shared_ptr<Cube> cube, const YGM::Transform& model) {
	curShader.SetMat4f("model", model.GetMatrix().Transpose());
	pGLWindow->GetVAO(CoreDefine::StaticVAOType::Cube).Draw(curShader);
}

void DeferredRaster::RenderMesh(std::shared_ptr<Plane> plane, const YGM::Transform& model) {
	curShader.SetMat4f("model", model.GetMatrix().Transpose());
	pGLWindow->GetVAO(CoreDefine::StaticVAOType::Plane).Draw(curShader);
}
