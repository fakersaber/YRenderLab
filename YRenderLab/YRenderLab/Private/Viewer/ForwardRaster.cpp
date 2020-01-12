#include <Public/Viewer/ForwardRaster.h>
#include <Public/Viewer/EnviromentGen.h>

#include <Public/OpenGLRHI/GlfwWindow.h>
#include <Public/OpenGLRHI/GLAD/glad/glad.h>

#include <Public/Scene/Scene.h>
#include <Public/Scene/Yobject.h>
#include <Public/Scene/MeshComponent.h>
#include <Public/Scene/TransformComponent.h>
#include <Public/Basic/MaterialComponent.h>

#include <Public/Basic/Mesh/TriMesh.h>
#include <Public/Basic/Image/Image.h>
#include <Public/Basic/BSDF_blinnPhong.h>
#include <Public/Basic/BSDF_StandardPBR.h>


ForwardRaster::ForwardRaster(std::shared_ptr<Scene> scene, std::shared_ptr<GlfwWindow> pGLWindow)
	:
	Raster(scene, New<EnviromentGen>(pGLWindow), pGLWindow)
{
}


void ForwardRaster::Draw() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	ObjectTransformVec.clear();
	ObjectTransformVec.emplace_back(1.f);

	UpdateUBO_Camera();
	UpdateUBO_DirectionalLights();
	UpdateUBO_Environment();
	
	enviromentGen->Visit(scene);
	Visit(scene->GetRoot());
	DrawEnvironment();
}

void ForwardRaster::Initial() {
	Raster::Initial();
	InitShader_Skybox();
	InitShaderStandardPBR();
}

void ForwardRaster::InitShaderStandardPBR() {
	new (&StandardPBRShader) GLShader("Data/shaders/P3N3T2T3.vs", "Data/shaders/materials/StandardPBR.fs");
	StandardPBRShader.SetInt("bsdf.albedoTexture", 0);
	StandardPBRShader.SetInt("bsdf.metallicTexture", 1);
	StandardPBRShader.SetInt("bsdf.roughnessTexture", 2);
	StandardPBRShader.SetInt("bsdf.aoTexture", 3);
	StandardPBRShader.SetInt("bsdf.normalTexture", 4);

	RegisterShader(StandardPBRShader, 5);
}


void ForwardRaster::InitShaderPbrBlinnPhong() {
	//BlinnPhongShader = GLShader("Data/shaders/P3N3T2T3.vs", "Data/shaders/materials/Pbr_Blinn_Phong.fs");
	//BlinnPhongShader.SetInt("bsdf.albedoTexture", 0);
	//BlinnPhongShader.SetInt("bsdf.normalTexture", 1);
	//BlinnPhongShader.SetInt("bsdf.specularTexture", 2);

	//RegisterShader(BlinnPhongShader, 3);
}

void ForwardRaster::InitShader_Skybox() {
	new (&shader_skybox) GLShader("Data/shaders/SkyBox/skybox.vs", "Data/shaders/SkyBox/skybox.fs");
	RegisterShader(shader_skybox, 0);
}


void ForwardRaster::Visit(std::shared_ptr<YObject> obj) {
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



void ForwardRaster::Visit(std::shared_ptr<BSDF_StandardPBR> bsdf) {
	SetCurShader(StandardPBRShader);
	std::string PreFix = "bsdf.";
	StandardPBRShader.SetVec3f(PreFix + "colorFactor", bsdf->colorFactor);
	StandardPBRShader.SetFloat(PreFix + "metallicFactor", bsdf->metallicFactor);
	StandardPBRShader.SetFloat(PreFix + "roughnessFactor", bsdf->roughnessFactor);

	const int texNum = 5;
	std::shared_ptr<Image> imgs[texNum] = { bsdf->albedoTexture, bsdf->metallicTexture, bsdf->roughnessTexture, bsdf->aoTexture, bsdf->normalTexture };
	//std::string names[texNum] = { "bsdf.haveAlbedoTexture","bsdf.haveNormalTexture","bsdf.haveSpecularTexture" };
	for (int i = 0; i < texNum; ++i) {
		if (imgs[i] && imgs[i]->IsValid()) {
			//StandardPBRShader.SetBool(names[i], true);
			pGLWindow->GetTexture(imgs[i]).Use(i);
		}
		//else {
		//	StandardPBRShader.SetBool(names[i], false);
		//}
	}

	UseLightTexureResource(StandardPBRShader);
}


void ForwardRaster::Visit(std::shared_ptr<BSDF_blinnPhong> bsdf) {
	//SetCurShader(BlinnPhongShader);
	//std::string PreFix = "bsdf.";
	//BlinnPhongShader.SetVec3f(PreFix + "colorFactor", bsdf->colorFactor);
	//BlinnPhongShader.SetFloat(PreFix + "gloss", bsdf->gloss);

	//const int texNum = 3;
	//std::shared_ptr<Image> imgs[texNum] = { bsdf->albedoTexture, bsdf->normalTexture,bsdf->specularTexture};
	//std::string names[texNum] = { "bsdf.haveAlbedoTexture","bsdf.haveNormalTexture","bsdf.haveSpecularTexture" };
	//for (int i = 0; i < texNum; ++i) {
	//	if (imgs[i] && imgs[i]->IsValid()) {
	//		BlinnPhongShader.SetBool(names[i], true);
	//		pGLWindow->GetTexture(imgs[i]).Use(i);
	//	}
	//	else {
	//		BlinnPhongShader.SetBool(names[i],false);
	//	}
	//}
}


void ForwardRaster::Visit(std::shared_ptr<TriMesh> mesh) {
	pGLWindow->GetVAO(mesh).Draw(curShader);
}

void ForwardRaster::RegisterShader(const GLShader& shader, const uint32_t TextureBase) {
	MapUBOToShader(shader);

	//if (depthmapBase < 0) // 无需计算光
	//	return;

	//shader2depthmapBase[shader] = depthmapBase;

	//shader.SetFloat("lightNear", lightNear);
	//shader.SetFloat("lightFar", lightFar);

	//// point light
	//int pointLightBase = depthmapBase;
	//for (int i = 0; i < maxPointLights; i++)
	//	shader.SetInt("pointLightDepthMap" + to_string(i), pointLightBase + i);

	//// directional light
	//int directionalLightBase = pointLightBase + maxPointLights;
	//for (int i = 0; i < maxDirectionalLights; i++)
	//	shader.SetInt("directionalLightDepthMap" + to_string(i), directionalLightBase + i);

	//// spot light
	//int spotLightBase = directionalLightBase + maxDirectionalLights;
	//for (int i = 0; i < maxSpotLights; i++)
	//	shader.SetInt("spotLightDepthMap" + to_string(i), spotLightBase + i);

	// environment
	const int environmentBase = TextureBase;
	shader.SetInt("skybox", environmentBase);
	shader.SetInt("irradianceMap", environmentBase + 1);
	shader.SetInt("prefilterMap", environmentBase + 2);
	shader.SetInt("brdfLUT", environmentBase + 3);
}

void ForwardRaster::UseLightTexureResource(const GLShader & shader) const {
	//auto target = shader2depthmapBase.find(shader);
	//if (target == shader2depthmapBase.cend()) {
	//	printf("ERROR::ForwardRaster::UseLightTex:\n"
	//		"\t""shader not regist\n");
	//	return;
	//}
	//const auto depthmapBase = target->second;


	//// point light
	//for (auto cmptLight : scene->GetCmptLights()) {
	//	auto pointLight = CastTo<PointLight>(cmptLight->light);
	//	auto target = pointLight2idx.find(pointLight);
	//	if (target == pointLight2idx.cend())
	//		continue;
	//	const auto pointLightIdx = target->second;

	//	pldmGenerator->GetDepthCubeMap(cmptLight).Use(depthmapBase + pointLightIdx);
	//}

	//// directional light
	//const int directionalLightBase = depthmapBase + maxPointLights;
	//for (auto cmptLight : scene->GetCmptLights()) {
	//	auto directionalLight = CastTo<DirectionalLight>(cmptLight->light);
	//	auto target = directionalLight2idx.find(directionalLight);
	//	if (target == directionalLight2idx.cend())
	//		continue;
	//	const auto directionalLightIdx = target->second;

	//	dldmGenerator->GetDepthMap(cmptLight).Use(directionalLightBase + directionalLightIdx);
	//}

	//// spot light
	//const int spotLightBase = directionalLightBase + maxDirectionalLights;
	//for (auto cmptLight : scene->GetCmptLights()) {
	//	auto spotLight = CastTo<SpotLight>(cmptLight->light);
	//	auto target = spotLight2idx.find(spotLight);
	//	if (target == spotLight2idx.cend())
	//		continue;
	//	const auto spotLightIdx = target->second;

	//	sldmGenerator->GetDepthMap(cmptLight).Use(spotLightBase + spotLightIdx);
	//}

	// environment
	auto environment = scene->GetEnviromentImg();
	const int environmentBase = 5;
	if (environment) {
		auto skybox = enviromentGen->GetSkyBox();
		skybox.Use(environmentBase);
		auto irradianceMap = enviromentGen->GetIrradianceMap();
		irradianceMap.Use(environmentBase + 1);
		auto prefilterMap = enviromentGen->GetPrefilterMap();
		prefilterMap.Use(environmentBase + 2);
		auto brdfLUT = enviromentGen->GetBRDF_LUT();
		brdfLUT.Use(environmentBase + 3);
	}
}


void ForwardRaster::DrawEnvironment() {
	if (!scene->GetEnviromentImg()) {
		printf("Error! ForwardRaster::DrawEnvironment \n");
		return;
	}

	glDepthFunc(GL_LEQUAL);
	UseLightTexureResource(shader_skybox);
	shader_skybox.SetBool("needGamma", true);
	pGLWindow->GetVAO(TriMesh::OriginCube).Draw(shader_skybox);
	glDepthFunc(GL_LESS);
}

