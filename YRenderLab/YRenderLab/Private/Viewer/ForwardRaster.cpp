#include <Public/Viewer/ForwardRaster.h>
#include <Public/OpenGLRHI/GLAD/glad/glad.h>

#include <Public/Scene/Scene.h>
#include <Public/Scene/Yobject.h>
#include <Public/Scene/MeshComponent.h>
#include <Public/Scene/TransformComponent.h>

#include <Public/Basic/Mesh/TriMesh.h>
#include <Public/Basic/MaterialComponent.h>
#include <Public/Basic/Image/Image.h>
#include <Public/Basic/BSDF_Diffuse.h>



namespace YRender {

	ForwardRaster::ForwardRaster(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera) : Raster(scene, camera){

	}


	void ForwardRaster::Draw(){
		glEnable(GL_DEPTH_TEST);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//UpdateShadowMap();
		//UpdateEnvironment();
		// 要放在深度图生成之后
		UpdateUBO();
		modelVec.clear();
		//modelVec.push_back(YGM::Transform(1.f));
		this->Visit(scene->GetRoot());

		//DrawEnvironment();
	}
	void ForwardRaster::Initial(){
		Raster::Initial();
		InitShaderDiffuse();
	}

	void ForwardRaster::InitShaderDiffuse() {
		DiffuseShader = GLShader("Data/shaders/Test.vs", "Data/shaders/Test.fs");
		DiffuseShader.SetInt("bsdf.albedoTexture", 0);
		//RegShader(shader_diffuse, 1);
		MapUBOToShader(DiffuseShader);
	}




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
			this->Visit(Cast<BSDF_Diffuse>(material->GetMaterial()));
		}

		if (mesh && mesh->GetMesh())
			this->Visit(mesh->GetMesh());

		for (auto child : children) {
			this->Visit(child);
		}
			
	}

	void ForwardRaster::Visit(std::shared_ptr<BSDF_Diffuse> bsdf) {
		SetCurShader(DiffuseShader);
		std::string DiffusePreFix = "bsdf.";
		DiffuseShader.SetVec3f(DiffusePreFix + "colorFactor", bsdf->colorFactor);
		auto target = img2tex.find(bsdf->albedoTexture);
		if (target == img2tex.end()) {
			GLTexture DiffuseAlbedoTexture(bsdf->albedoTexture);
			img2tex[bsdf->albedoTexture] = DiffuseAlbedoTexture;
			DiffuseAlbedoTexture.Use(0);
		}
		else {
			target->second.Use(0);
		}
	}

	void ForwardRaster::Visit(std::shared_ptr<TriMesh> mesh) {
		//curShader.SetMat4f("model", modelVec.back());

		auto TargetVAO = mesh2VAO.find(mesh);
		if (TargetVAO == mesh2VAO.end()) {
			std::vector<VAO::VBO_DataPatch> Mesh_VAO_DataPatch = {
			{mesh->GetPositions().data()->Data(), static_cast<unsigned int>(mesh->GetPositions().size() * 3 * sizeof(float)), 3},
			{ mesh->GetNormals().data()->Data(), static_cast<unsigned int>(mesh->GetNormals().size() * 3 * sizeof(float)), 3 },
			{ mesh->GetTexcoords().data()->Data(), static_cast<unsigned int>(mesh->GetTexcoords().size() * 2 * sizeof(float)), 2 },
			/*{ mesh->GetTangents().data()->Data(), static_cast<unsigned int>(mesh->GetTangents().size() * 3 * sizeof(float)), 3 },*/};
			VAO VAO_P3N3T2T3_Mesh(Mesh_VAO_DataPatch, mesh->GetIndice().data(), static_cast<unsigned int>(mesh->GetIndice().size() * sizeof(unsigned int)));
			mesh2VAO[mesh] = VAO_P3N3T2T3_Mesh;
			VAO_P3N3T2T3_Mesh.Draw(curShader);
		}
		else {
			TargetVAO->second.Draw(curShader);
		}
	}
}

