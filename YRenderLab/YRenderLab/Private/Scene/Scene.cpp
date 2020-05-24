#include <Public/Scene/Scene.h>
#include <Public/Basic/Camera/Camera.h>
#include <Public/Scene/Yobject.h>
#include <Public/Scene/LightComponent.h>

#include <Public/Scene/AssimpLoader.h>
#include <Public/Lights/DirectionalLight.h>
#include <Public/Scene/LightComponent.h>
#include <Public/Scene/TransformComponent.h>
#include <Public/Scene/MeshComponent.h>
#include <Public/Basic/Mesh/Plane.h>

#include <Public/Basic/Image/Image.h>

#include <Public/Basic/BSDF_Emission.h>
#include <Public/Basic/MaterialComponent.h>


Scene::Scene(){
	root = AssimpLoader::Load("Data/module/Cerberus_by_Andrew_Maximov/TargetModule.FBX");
	EnviromentImg = New<Image>("Data/module/Arches_E_PineTree/Arches_E_PineTree_3k.hdr");
	camera = New<Camera>();
	
	GenerateLights();

	//create Plane
	{
		auto Plane_0 = New<YObject>("Plane_0", root);
		New<MeshComponent>(Plane_0, New<Plane>());
		auto PlaneTransform = New<TransformComponent>(Plane_0);
		//顺序必须是缩放，旋转，平移---先平移再缩放会影响平移值
		PlaneTransform->SetWorldScale(Vector3(20.f, 20.f, 20.f));
		PlaneTransform->RotateX(90.f);
		PlaneTransform->SetWorldTranslate(Vector3(0.f, 0.f, -60.f));

		New<MaterialComponent>(Plane_0, New<BSDF_Emission>());
	}

	root->PrintNode();

}


void Scene::GenerateLights(){
	assert(root);
	auto light_0 = New<YObject>("light_0", root);
	New<TransformComponent>(light_0);
	New<LightComponent>(light_0, New<DirectionalLight>(RGBf::White, 1.f));
}

void Scene::GenerateTerrain(){

}


std::vector<std::shared_ptr<LightComponent>> Scene::GetLightComponents() const {
	return root->GetComponentsInChildren<LightComponent>();
}

