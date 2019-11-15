#include <Public/Viewer/ForwardRaster.h>
#include <Public/OpenGLRHI/GLAD/glad/glad.h>
#include <Public/OpenGLRHI/GLShader.h>

#include <Public/Scene/Scene.h>
#include <Public/Scene/Yobject.h>
#include <Public/Scene/MeshComponent.h>
#include <Public/Scene/TransformComponent.h>
#include <Public/Basic/MaterialComponent.h>

#include <Public/Basic/BSDF_Diffuse.h>

namespace YRender {
	void ForwardRaster::Draw(){
		glEnable(GL_DEPTH_TEST);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//UpdateShadowMap();
		//UpdateEnvironment();
		// Ҫ�������ͼ����֮��
		UpdateUBO();
		modelVec.clear();
		//modelVec.push_back(YGM::Transform(1.f));
		this->Visit(scene->GetRoot());

		//DrawEnvironment();
	}
	void ForwardRaster::Visit(std::shared_ptr<YObject> obj){
		auto mesh = obj->GetComponent<MeshComponent>();
		auto material = obj->GetComponent<MaterialComponent>();
		auto children = obj->GetChildren();

		if ((!mesh || !mesh->GetMesh() || !material || !material->GetMaterial()) && children.empty())
			return;
		auto transform = obj->GetComponent<TransformComponent>();
		if (transform != nullptr) {
			//����û���Ը��ڵ��transformΪ��
			modelVec.push_back(transform->GetTransform());
		}

		if (mesh && mesh->GetMesh())
			this->Visit(mesh->GetMesh());

		if (material && material->GetMaterial()) {
			//�ȶ�����Ϊ������ʣ�֮������дÿ����mesh����material�Լ����ù��ߵĺ���
			this->Visit(Cast<BSDF_Diffuse>(material->GetMaterial()));
		}

		for (auto child : children) {
			this->Visit(child);
		}
			
	}

	void ForwardRaster::Visit(std::shared_ptr<BSDF_Diffuse> bsdf) {

	}

	void ForwardRaster::Visit(std::shared_ptr<TriMesh> mesh) {
		//curShader.SetMat4f("model", modelVec.back());
		auto TargetVAO = mesh2VAO.find(mesh);
		if (TargetVAO == mesh2VAO.end()) {
			std::vector<VAO::VBO_DataPatch> Mesh_VAO_DataPatch = {
			{mesh->GetPositions().data()->Data(), static_cast<unsigned int>(mesh->GetPositions().size() * 3 * sizeof(float)), 3},
			{ mesh->GetNormals().data()->Data(), static_cast<unsigned int>(mesh->GetNormals().size() * 3 * sizeof(float)), 3 },
			{ mesh->GetTexcoords().data()->Data(), static_cast<unsigned int>(mesh->GetTexcoords().size() * 2 * sizeof(float)), 2 },
			{ mesh->GetTangents().data()->Data(), static_cast<unsigned int>(mesh->GetTangents().size() * 3 * sizeof(float)), 3 },};

			VAO VAO_P3N3T2T3_Mesh(Mesh_VAO_DataPatch, mesh->GetIndice().data(), static_cast<unsigned int>(mesh->GetIndice().size() * sizeof(unsigned int)));
			mesh2VAO[mesh] = VAO_P3N3T2T3_Mesh;
		}
		else {
			TargetVAO->second.Draw(curShader);
		}
	}
}
