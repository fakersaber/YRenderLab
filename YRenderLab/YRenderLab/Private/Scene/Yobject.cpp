#include <Public/Scene/Yobject.h>

#include <Public/Scene/Component.h>
#include <Public/Scene/TransformComponent.h>

void YObject::AttachComponent(const std::shared_ptr<Component> component) {
	auto iter = components.find(typeid(*component));
	if (iter != components.end()) {
		iter->second->OwnerObj.reset();
	}
	component->OwnerObj = weak_this<YObject>();
	components[typeid(*component)] = component;
}


void YObject::DetachComponent(const std::shared_ptr<Component> component) {
	auto iter = components.find(typeid(*component));
	if (iter == components.end())
		return;
	if (iter->second != component)
		return;
	component->OwnerObj.reset();
	components.erase(iter);
}

Vector3 YObject::GetObjectWorldForward(){
	const auto& ObjectTransform = GetComponent<TransformComponent>()->GetWorldTransform();
	Vector3 RetVal = ObjectTransform.TrasformVec(TransformComponent::Forward);
	return RetVal.Normalize();
}

