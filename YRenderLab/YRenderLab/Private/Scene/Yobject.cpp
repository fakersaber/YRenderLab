#include <Public/Scene/Yobject.h>

#include <Public/Basic/Component.h>

namespace YRender {
	void YObject::AttachComponent(const std::shared_ptr<Component>& component){
		component->OwnerObj = weak_this<YObject>();
		components.insert(component);
	}


	void YObject::DetachComponent(const std::shared_ptr<Component>& component){
		auto iter = components.find(component);
		if (iter != components.end()) {	
			components.erase(iter);
		}
	}
}