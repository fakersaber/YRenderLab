#include <Public/Scene/TransformComponent.h>
#include <Public/Scene/Yobject.h>

const Vector3 TransformComponent::Forward(0.f,0.f,-1.f);
const Vector3 TransformComponent::Up(0.f, 1.f, 0.f);
const Vector3 TransformComponent::Right(1.f, 0.f, 0.f);


TransformComponent::TransformComponent(const std::shared_ptr<YObject> obj,const YGM::Transform& RelativeTransform) : Component(obj){
	this->RelativeTransform = RelativeTransform;
	if (auto parent = obj->GetParent()) {
		const auto& ParentWorldMatrix = parent->GetComponent<TransformComponent>()->GetWorldTransform().GetMatrix();
		WorldTransform = YGM::Transform(ParentWorldMatrix * this->RelativeTransform.GetMatrix());
	}
	else {
		WorldTransform = this->RelativeTransform;
	}
}

void TransformComponent::SetWorldScale(const Vector3& scale){
	//#TODO: 实现应该是要判断parent然后计算Relative后SetRelativeScale，SetRelativeScale里面更新world
	//child需要同时设置
	auto& WorldMatrix = WorldTransform.GetMatrix();
	WorldMatrix(0, 0) *= scale.x;
	WorldMatrix(1, 1) *= scale.y;
	WorldMatrix(2, 2) *= scale.z;
}

void TransformComponent::SetRelativeScale(const Vector3& scale){

}
