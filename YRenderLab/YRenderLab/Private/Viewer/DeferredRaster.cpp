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
#include <Public/Scene/LightComponent.h>
#include <Public/Lights/DirectionLight.h>
#include <Public/Lights/PointLight.h>

DeferredRaster::DeferredRaster(std::shared_ptr<Scene> scene, std::shared_ptr<GlfwWindow> pGLWindow)
	:
	Raster(scene, New<EnviromentGen>(pGLWindow), pGLWindow)
{
}

void DeferredRaster::Draw() {
	glEnable(GL_DEPTH_TEST);

	//Update UBO
	{
		UpdateUBO_Camera();
		UpdateUBO_DirectionalLights();
		UpdateUBO_Environment();
	}

	//Update data that like  lut etc...
	{
		enviromentGen->Visit(scene);
	}

	{
		Pass_GBuffer();

		Pass_Lights();

		Pass_AmbientLight();

		Pass_SkyBox();

		Pass_TAA();

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
				GLTexture::TexTureformat::TEX_GL_RGBA32F  //albedo + ao
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

void DeferredRaster::InitShader_Lights() {
	new (&DirectLight_Shader) GLShader("Data/shaders/P2T2.vs", "Data/shaders/DeferredPipline/DirectLight.fs");
	int StartIndex = 0;
	DirectLight_Shader.SetInt("GBuffer0", StartIndex++);
	DirectLight_Shader.SetInt("GBuffer1", StartIndex++);
	DirectLight_Shader.SetInt("GBuffer2", StartIndex++);

	MapUBOToShader(DirectLight_Shader);
}

void DeferredRaster::InitShader_AmbientLight() {

}

void DeferredRaster::InitShader_Skybox() {
	new (&Skybox_Shader) GLShader("Data/shaders/SkyBox/skybox.vs", "Data/shaders/SkyBox/skybox.fs");
	Skybox_Shader.SetInt("skybox", 0);
	MapUBOToShader(Skybox_Shader);
}


void DeferredRaster::InitShader_TAA() {

}

void DeferredRaster::InitShader_PostProcess() {

}



void DeferredRaster::Pass_GBuffer() {
	gbufferFBO.Use();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Reset transform
	ObjectTransformVec.clear();
	ObjectTransformVec.emplace_back(1.f);

	this->Visit(scene->GetRoot());
}

void DeferredRaster::Pass_Lights() {
	glDisable(GL_DEPTH_TEST);
	windowFBO.Use();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// set gbuffer
	gbufferFBO.GetColorTexture(0).Use(0);
	gbufferFBO.GetColorTexture(1).Use(1);
	gbufferFBO.GetColorTexture(2).Use(2);
	//gbufferFBO.GetColorTexture(3).Use(3);

	//set point light
	//for (auto cmptLight : scene->GetCmptLights()) {
	//	auto pointLight = Cast<PointLight>(cmptLight->light);
	//	auto target = pointLight2idx.find(pointLight);
	//	if (target == pointLight2idx.cend())
	//		continue;
	//	const auto pointLightIdx = target->second;

	//	pldmGenerator->GetDepthCubeMap(cmptLight).Use(4 + pointLightIdx);
	//}

	// set directional light
	//const int directionalLightBase = maxPointLights;
	//for (auto cmptLight : scene->GetLightComponents()) {
	//	auto directionalLight = Cast<DirectionLight>(cmptLight->GetLight());
	//	auto target = directionalLight2idx.find(directionalLight);
	//	if (target == directionalLight2idx.cend())
	//		continue;
	//	const auto directionalLightIdx = target->second;

	//	dldmGenerator->GetDepthMap(cmptLight).Use(directionalLightIdx);
	//}

	// set spot light
	//const int spotLightBase = directionalLightBase + maxDirectionalLights;
	//for (auto cmptLight : scene->GetCmptLights()) {
	//	auto spotLight = CastTo<SpotLight>(cmptLight->light);
	//	auto target = spotLight2idx.find(spotLight);
	//	if (target == spotLight2idx.cend())
	//		continue;
	//	const auto spotLightIdx = target->second;

	//	sldmGenerator->GetDepthMap(cmptLight).Use(spotLightBase + spotLightIdx);
	//}

	// ltc
	//const int ltcBase = spotLightBase + maxSpotLights;
	//ltcTex1.Use(ltcBase);
	//ltcTex2.Use(ltcBase + 1);

	pGLWindow->GetVAO(GlfwWindow::VAOTYPE::Screen).Draw(DirectLight_Shader);
	//pOGLW->GetVAO(ShapeType::Screen).Draw(directLightShader);


	glEnable(GL_DEPTH_TEST);
}



//两个计算光照的Pass都不用开depth，因为前面的pass已经算过一次深度信息了
void DeferredRaster::Pass_AmbientLight() {
	glDisable(GL_DEPTH_TEST);

	//环境光需要叠加解析光
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	windowFBO.Use();
	gbufferFBO.GetColorTexture(0).Use(0);
	gbufferFBO.GetColorTexture(1).Use(1);
	gbufferFBO.GetColorTexture(2).Use(2);

	const int environmentBase = 3;

	auto environment = scene->GetEnviromentImg();
	if (environment) {
		//auto skybox = enviromentGen->GetSkyBox();
		//skybox.Use(environmentBase);
		auto irradianceMap = enviromentGen->GetIrradianceMap();
		irradianceMap.Use(environmentBase);
		auto prefilterMap = enviromentGen->GetPrefilterMap();
		prefilterMap.Use(environmentBase + 1);
		auto brdfLUT = enviromentGen->GetBRDF_LUT();
		brdfLUT.Use(environmentBase + 2);
	}

	pGLWindow->GetVAO(GlfwWindow::VAOTYPE::Screen).Draw();
	glEnable(GL_DEPTH_TEST);
}


void DeferredRaster::Pass_SkyBox() {

}

void DeferredRaster::Pass_TAA() {

}

void DeferredRaster::Pass_PostProcess()
{
}



void DeferredRaster::Visit(std::shared_ptr<YObject> obj) {
	auto mesh = obj->GetComponent<MeshComponent>();
	auto material = obj->GetComponent<MaterialComponent>();

	auto transform = obj->GetComponent<TransformComponent>();
	if (transform != nullptr) {
		//这里没有以父节点的transform为基
		ObjectTransformVec.push_back(/*modelVec.back() * */ transform->GetTransform());
	}

	if (material && material->GetMaterial() && mesh && mesh->GetMesh()) {
		this->Visit(Cast<BSDF_StandardPBR>(material->GetMaterial()));
		this->Visit(mesh->GetMesh());
	}

	for (auto child : obj->GetChildrens()) {
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
	std::shared_ptr<Image> imgs[texNum] = {
		material->albedoTexture,
		material->metallicTexture,
		material->roughnessTexture,
		material->aoTexture,
		material->normalTexture
	};
	for (int i = 0; i < texNum; ++i) {
		if (imgs[i] && imgs[i]->IsValid()) {
			pGLWindow->GetTexture(imgs[i]).Use(i);
		}
	}
}


void DeferredRaster::Visit(std::shared_ptr<TriMesh> mesh) {
	pGLWindow->GetVAO(mesh).Draw(curShader);
}