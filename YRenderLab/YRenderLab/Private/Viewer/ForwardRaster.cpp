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




namespace YRender {

	//ForwardRaster::CubeVAO = VAO(
	//	std::vector<VAO::VBO_DataPatch>{ {}},
	//	TriMesh::OriginCube->GetIndice().data(), 
	//	static_cast<unsigned int>(TriMesh::OriginCube->GetIndice().size() * sizeof(unsigned int))
	//);

	ForwardRaster::ForwardRaster(std::shared_ptr<Scene> scene,std::shared_ptr<GlfwWindow> pGLWindow) 
		: 
		Raster(scene,YRender::New<EnviromentGen>(pGLWindow), pGLWindow)
	{
	}


	void ForwardRaster::Draw(){
		glEnable(GL_DEPTH_TEST);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//UpdateShadowMap();
		UpdateEnvironment();
		UpdateUBO();
		modelVec.clear();
		//modelVec.push_back(YGM::Transform(1.f));
		this->Visit(scene->GetRoot());
		//最后draw environment减少overdraw
		DrawEnvironment();
	}
	void ForwardRaster::Initial(){
		Raster::Initial();
		InitShader_Skybox();
		InitShaderPbrBlinnPhong();
		//InitShaderDiffuseSpecular();
	}

	void ForwardRaster::InitShaderPbrBlinnPhong() {
		BlinnPhongShader = GLShader("Data/shaders/P3N3T2T3.vs", "Data/shaders/Pbr_Blinn_Phong/Pbr_Blinn_Phong.fs");
		BlinnPhongShader.SetInt("bsdf.albedoTexture", 0);
		BlinnPhongShader.SetInt("bsdf.normalTexture", 1);
		BlinnPhongShader.SetInt("bsdf.specularTexture", 2);
		MapUBOToShader(BlinnPhongShader);
	}

	void ForwardRaster::InitShader_Skybox(){
		shader_skybox = GLShader("Data/shaders/SkyBox/skybox.vs", "Data/shaders/SkyBox/skybox.fs");
		shader_skybox.SetInt("skybox", 0);
		MapUBOToShader(shader_skybox);
	}

	//void ForwardRaster::InitShaderDiffuseSpecular(){
	//	DiffuseSpecular = GLShader("Data/shaders/P3N3T2.vs", "Data/shaders/Diffuse_Specular/Diffuse_Specular.fs");
	//	DiffuseSpecular.SetInt("bsdf.albedoTexture", 0);
	//	DiffuseSpecular.SetInt("bsdf.normalTexture", 1);
	//	DiffuseSpecular.SetInt("bsdf.specularTexture", 2);
	//	MapUBOToShader(DiffuseSpecular);
	//}

	void ForwardRaster::Visit(std::shared_ptr<YObject> obj){
		auto mesh = obj->GetComponent<MeshComponent>();
		auto material = obj->GetComponent<MaterialComponent>();
		auto children = obj->GetChildren();

		if ((!mesh || !mesh->GetMesh() || !material || !material->GetMaterial()) && children.empty())
			return;
		auto transform = obj->GetComponent<TransformComponent>();
		if (transform != nullptr) {
			//这里没有以父节点的transform为基
			modelVec.push_back(transform->GetTransform());
		}

		if (material && material->GetMaterial()) {
			//先都设置为这个材质，之后再重写每个让mesh或者material自己调用管线的函数
			this->Visit(Cast<BSDF_blinnPhong>(material->GetMaterial()));
		}

		if (mesh && mesh->GetMesh())
			this->Visit(mesh->GetMesh());

		for (auto child : children) {
			this->Visit(child);
		}	
	}

	void ForwardRaster::Visit(std::shared_ptr<BSDF_blinnPhong> bsdf) {
		SetCurShader(BlinnPhongShader);
		std::string PreFix = "bsdf.";
		BlinnPhongShader.SetVec3f(PreFix + "colorFactor", bsdf->colorFactor);
		BlinnPhongShader.SetFloat(PreFix + "gloss", bsdf->gloss);

		const int texNum = 3;
		std::shared_ptr<Image> imgs[texNum] = { bsdf->albedoTexture, bsdf->normalTexture,bsdf->specularTexture};
		std::string names[texNum] = { "bsdf.haveAlbedoTexture","bsdf.haveNormalTexture","bsdf.haveSpecularTexture" };
		for (int i = 0; i < texNum; ++i) {
			if (imgs[i] && imgs[i]->IsValid()) {
				BlinnPhongShader.SetBool(names[i], true);
				pGLWindow->GetTexture(imgs[i]).Use(i);
			}
			else {
				BlinnPhongShader.SetBool(names[i],false);
			}
		}
	}


	void ForwardRaster::Visit(std::shared_ptr<TriMesh> mesh) {
		pGLWindow->GetVAO(mesh).Draw(curShader);
	}


	void ForwardRaster::DrawEnvironment(){
		glDepthFunc(GL_LEQUAL);


		//enviromentGen->GetSkyBox()->Use(0);
		//enviromentGen->shader_genIBLSkybox.SetMat4f("Camera.view", this->scene->GetCamera()->GetViewMatrix().Transpose());
		//enviromentGen->shader_genIBLSkybox.SetMat4f("Camera.projection", this->scene->GetCamera()->GetProjectMatrix().Transpose());
		//pGLWindow->GetVAO(TriMesh::OriginCube).Draw(enviromentGen->shader_genIBLSkybox);
		glDepthFunc(GL_LESS);
	}
}

