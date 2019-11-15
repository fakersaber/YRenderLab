#include <Public/Scene/Yobject.h>

#include <Public/Scene/Component.h>

namespace YRender {
	void YObject::AttachComponent(const std::shared_ptr<Component>& component){
		auto iter = components.find(typeid(*component));
		if (iter != components.end()) {
			iter->second->OwnerObj.reset();
		}
		component->OwnerObj = weak_this<YObject>();
		components[typeid(*component)] = component;
	}


	void YObject::DetachComponent(const std::shared_ptr<Component>& component){
		auto iter = components.find(typeid(*component));
		if (iter == components.end())
			return;
		if (iter->second != component)
			return;
		component->OwnerObj.reset();
		components.erase(iter);
	}
}